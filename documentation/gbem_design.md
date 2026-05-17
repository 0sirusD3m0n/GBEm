# Game Boy Emulator (gbem) — Design Document

---

## 1. Overview

gbem is a Game Boy (DMG) emulator targeting functional accuracy at the M-cycle level. The goal is to run the majority of the commercial Game Boy library correctly, with a clean architecture that allows T-cycle accuracy to be added later if desired.

**Target hardware:** Sharp SM83 CPU, 4.194304 MHz clock
**Timing model:** M-cycle (1.048576 MHz, ~17556 M-cycles per frame at 59.7275 fps)

---

## 2. File Structure

```
gbem/
├── gb.h/c              - Top level, owns gb_t, init/free/run
├── cpu/
│   ├── cpu.h/c         - CPU state, fetch, main execution loop
│   ├── decode.h/c      - Opcode tables, x/y/z decode helpers, register tables
│   └── handlers/
│       ├── ld.h/c      - All load instructions
│       ├── alu.h/c     - ADD, ADC, SUB, SBC, AND, XOR, OR, CP
│       ├── control.h/c - JP, JR, CALL, RET, RST
│       ├── bit.h/c     - CB prefix handlers (BIT, RES, SET, shifts, rotates)
│       └── misc.h/c    - NOP, HALT, STOP, DAA, CPL, SCF, CCF, EI, DI
├── ppu/
│   ├── ppu.h/c         - Pixel processing unit, mode transitions
│   └── lcd.h/c         - LCD output, framebuffer
├── memory/
│   ├── bus.h/c         - Memory map, read/write routing
│   ├── ram.h/c         - WRAM, HRAM
│   └── dma.h/c         - OAM DMA transfer
├── cart/
│   ├── cart.h/c        - Cartridge header parsing, ROM loading
│   └── mbc.h/c         - Memory bank controllers (MBC1, MBC2, MBC3, MBC5)
├── timer.h/c           - DIV, TIMA, TMA, TAC registers
├── interrupt.h/c       - IE, IF, IME handling
├── joypad.h/c          - Input handling
└── apu/
    ├── apu.h/c         - Audio processing unit
    └── channels.h/c    - Individual sound channels (CH1-CH4)
```

---

## 3. Top Level Architecture

Everything is owned by a single `gb_t` struct. Components communicate through this struct and the bus — nothing reaches across components directly.

```c
typedef struct {
    gb_cpu_t        cpu;
    gb_ram_t        ram;
    gb_ppu_t        ppu;
    gb_cart_t       cart;
    gb_timer_t      timer;
    gb_interrupt_t  interrupts;
    gb_joypad_t     joypad;
    gb_apu_t        apu;
} gb_t;
```

### Lifecycle

```c
gb_error_t  gb_init(gb_t *gb, const char *rom_path);
void        gb_free(gb_t *gb);
void        gb_tick(gb_t *gb);        // advance one M-cycle
void        gb_run_frame(gb_t *gb);   // advance one full frame (17556 M-cycles)
```

`gb_init` initializes components in dependency order - cartridge first, then bus, then CPU and peripherals. `gb_free` cleans up any heap allocations (ROM data etc).

### Main Loop

```c
void gb_run_frame(gb_t *gb) {
    int cycles = 0;
    while (cycles < M_CYCLES_PER_FRAME) {
        cycles += cpu_step(&gb->cpu);
        ppu_tick(&gb->ppu, cycles);
        timer_tick(&gb->timer, cycles);
        apu_tick(&gb->apu, cycles);
    }
    render_frame(&gb->ppu);
}
```

Frame timing is synchronized to real time after each frame is completed, using timestamps to sleep for the remaining frame budget (~16.74ms at 59.7275 fps).

---

## 4. CPU

### State

```c
typedef struct {
    gb_register_t af;   // accumulator + flags
    gb_register_t bc;
    gb_register_t de;
    gb_register_t hl;
    uint16_t      sp;   // stack pointer
    uint16_t      pc;   // program counter
    uint8_t       ir;   // instruction register (current opcode)
    bool          ime;  // interrupt master enable
    bool          halted;
    bool          stopped;
} gb_cpu_t;
```

### Register Type

All register pairs are modeled using a union that provides three simultaneous views of the same memory: the full 16-bit word, the two individual bytes, and (for AF) the individual flag bits.

```c
typedef union {
    uint16_t word;
    struct {
        union {
            uint8_t low;
            struct {
                uint8_t : 4;           // bits 0-3, always 0
                uint8_t carry      : 1; // bit 4
                uint8_t half_carry : 1; // bit 5
                uint8_t negative   : 1; // bit 6
                uint8_t zero       : 1; // bit 7
            };
        };
        uint8_t high;
    };
} gb_register_t;
```

### Fetch

Each fetch reads one byte from memory at `pc` and increments `pc`. The opcode is the first fetch; additional bytes (immediates, CB opcode) are fetched by the handler.

```c
uint8_t fetch(gb_cpu_t *cpu) {
    return mem_read(cpu->bus, cpu->pc++);
}

uint16_t fetch16(gb_cpu_t *cpu) {
    uint8_t low  = fetch(cpu);
    uint8_t high = fetch(cpu);
    return (high << 8) | low;
}
```

Each fetch consumes one M-cycle, so instruction costs are:

| Instruction type       | Fetches | M-cycles | T-cycles |
|------------------------|---------|----------|----------|
| No immediate           | 1       | 1        | 4        |
| 8-bit immediate        | 2       | 2        | 8        |
| 16-bit immediate       | 3       | 3        | 12       |
| CB-prefixed            | 2       | 2+       | 8+       |

---

## 5. Decode

### Opcode Tables

Two separate function pointer tables, one per opcode space. Both are indexed directly by the opcode byte value.

```c
typedef void (*gb_opcode_handler_t)(gb_cpu_t *cpu);

gb_opcode_handler_t uc_table[256];  // unprefixed
gb_opcode_handler_t cb_table[256];  // 0xCB prefixed
```

Dispatch is a direct table lookup — no switch statements, no branching:

```c
int cpu_step(gb_cpu_t *cpu) {
    cpu->ir = fetch(cpu);
    uc_table[cpu->ir](cpu);
    return cpu->cycles; // M-cycles consumed
}
```

The CB prefix handler fetches the second byte and dispatches into `cb_table`:

```c
void handle_prefix_cb(gb_cpu_t *cpu) {
    cpu->ir = fetch(cpu);
    cb_table[cpu->ir](cpu);
}
```

### x/y/z Decode

The opcode byte is structured as:

```
[ x : 2 ][ y : 3 ][ z : 3 ]
          [ p : 2 ][ q : 1 ]
```

Decoded as:

```c
uint8_t x = opcode >> 6;
uint8_t y = (opcode >> 3) & 0x7;
uint8_t z = opcode & 0x7;
uint8_t p = y >> 1;
uint8_t q = y & 0x1;
```

Handlers decode their own `ir` field to determine src/dst registers rather than having the opcode passed as a parameter.

### Register Tables

Two pointer arrays provide register operand lookup. Both point directly into the CPU struct — reads and writes go to the actual registers with no copying.

```c
// 8-bit register table, indexed by 3-bit z or y field
uint8_t *r[8] = {
    &cpu->bc.high,  // B - 0
    &cpu->bc.low,   // C - 1
    &cpu->de.high,  // D - 2
    &cpu->de.low,   // E - 3
    &cpu->hl.high,  // H - 4
    &cpu->hl.low,   // L - 5
    NULL,           // (HL) - 6, memory indirect, handled explicitly
    &cpu->af.high,  // A - 7
};

// 16-bit register pair table, indexed by 2-bit p field
uint16_t *rp[4] = {
    &cpu->bc.word,  // BC - 0
    &cpu->de.word,  // DE - 1
    &cpu->hl.word,  // HL - 2
    &cpu->sp,       // SP - 3
};

// 16-bit register pair table (AF variant, used by PUSH/POP)
uint16_t *rp2[4] = {
    &cpu->bc.word,  // BC - 0
    &cpu->de.word,  // DE - 1
    &cpu->hl.word,  // HL - 2
    &cpu->af.word,  // AF - 3
};
```

Index 6 of `r[]` is the `(HL)` indirect case and must be handled explicitly in any handler that uses the register table:

```c
uint8_t read_r(gb_cpu_t *cpu, uint8_t idx) {
    if (idx == 6) return mem_read(cpu->bus, cpu->hl.word);
    return *r[idx];
}

void write_r(gb_cpu_t *cpu, uint8_t idx, uint8_t val) {
    if (idx == 6) mem_write(cpu->bus, cpu->hl.word, val);
    else *r[idx] = val;
}
```

---

## 6. Opcode Handlers

All ~512 opcodes are covered by approximately 40 unique handler functions. Register and condition variation within each group is handled by decoding the `ir` field rather than creating separate handlers per register.

### Unprefixed Handlers (~30)

| Handler | Opcodes covered |
|---|---|
| `handle_nop` | 0x00 |
| `handle_stop` | 0x10 |
| `handle_halt` | 0x76 |
| `handle_ld_rp_nn` | 0x01, 0x11, 0x21, 0x31 |
| `handle_ld_r_n` | 0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x36, 0x3E |
| `handle_ld_r_r` | 0x40-0x7F (minus 0x76) |
| `handle_ld_rpi_a` | 0x02, 0x12 |
| `handle_ld_a_rpi` | 0x0A, 0x1A |
| `handle_ld_hlii_a` | 0x22 |
| `handle_ld_hlid_a` | 0x32 |
| `handle_ld_a_hlii` | 0x2A |
| `handle_ld_a_hlid` | 0x3A |
| `handle_ld_nn_sp` | 0x08 |
| `handle_ld_sp_hl` | 0xF9 |
| `handle_ld_hl_sp_e` | 0xF8 |
| `handle_ld_ff00_n_a` | 0xE0 |
| `handle_ld_a_ff00_n` | 0xF0 |
| `handle_ld_ff00_c_a` | 0xE2 |
| `handle_ld_a_ff00_c` | 0xF2 |
| `handle_ld_nn_a` | 0xEA |
| `handle_ld_a_nn` | 0xFA |
| `handle_inc_r` | 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C |
| `handle_dec_r` | 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D |
| `handle_inc_rp` | 0x03, 0x13, 0x23, 0x33 |
| `handle_dec_rp` | 0x0B, 0x1B, 0x2B, 0x3B |
| `handle_add_hl_rp` | 0x09, 0x19, 0x29, 0x39 |
| `handle_add_sp_e` | 0xE8 |
| `handle_alu_r` | 0x80-0xBF |
| `handle_alu_n` | 0xC6, 0xCE, 0xD6, 0xDE, 0xE6, 0xEE, 0xF6, 0xFE |
| `handle_jp` | 0xC3 |
| `handle_jp_cc` | 0xC2, 0xCA, 0xD2, 0xDA |
| `handle_jp_hl` | 0xE9 |
| `handle_jr` | 0x18 |
| `handle_jr_cc` | 0x20, 0x28, 0x30, 0x38 |
| `handle_call` | 0xCD |
| `handle_call_cc` | 0xC4, 0xCC, 0xD4, 0xDC |
| `handle_ret` | 0xC9 |
| `handle_ret_cc` | 0xC0, 0xC8, 0xD0, 0xD8 |
| `handle_reti` | 0xD9 |
| `handle_push` | 0xC5, 0xD5, 0xE5, 0xF5 |
| `handle_pop` | 0xC1, 0xD1, 0xE1, 0xF1 |
| `handle_rst` | 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF |
| `handle_rlca` | 0x07 |
| `handle_rrca` | 0x0F |
| `handle_rla` | 0x17 |
| `handle_rra` | 0x1F |
| `handle_daa` | 0x27 |
| `handle_cpl` | 0x2F |
| `handle_scf` | 0x37 |
| `handle_ccf` | 0x3F |
| `handle_ei` | 0xFB |
| `handle_di` | 0xF3 |
| `handle_prefix_cb` | 0xCB |

### CB-Prefixed Handlers (~8)

All CB handlers decode `y` for the bit number and `z` for the register operand.

| Handler | CB opcodes covered |
|---|---|
| `handle_cb_rlc` | 0x00-0x07 |
| `handle_cb_rrc` | 0x08-0x0F |
| `handle_cb_rl` | 0x10-0x17 |
| `handle_cb_rr` | 0x18-0x1F |
| `handle_cb_sla` | 0x20-0x27 |
| `handle_cb_sra` | 0x28-0x2F |
| `handle_cb_swap` | 0x30-0x37 |
| `handle_cb_srl` | 0x38-0x3F |
| `handle_cb_bit` | 0x40-0x7F |
| `handle_cb_res` | 0x80-0xBF |
| `handle_cb_set` | 0xC0-0xFF |

---

## 7. Memory Bus

All memory access goes through the bus. The CPU never touches memory directly.

```c
uint8_t  mem_read(gb_bus_t *bus, uint16_t addr);
void     mem_write(gb_bus_t *bus, uint16_t addr, uint8_t val);
```

The bus routes reads and writes based on address range:

| Address range | Maps to |
|---|---|
| 0x0000-0x7FFF | Cartridge ROM (via MBC) |
| 0x8000-0x9FFF | VRAM |
| 0xA000-0xBFFF | Cartridge RAM (via MBC) |
| 0xC000-0xDFFF | WRAM |
| 0xE000-0xFDFF | WRAM echo |
| 0xFE00-0xFE9F | OAM |
| 0xFF00-0xFF7F | IO registers |
| 0xFF80-0xFFFE | HRAM |
| 0xFFFF | IE register |

---

## 8. Interrupts

```c
typedef struct {
    uint8_t ie;   // 0xFFFF - interrupt enable
    uint8_t if_;  // 0xFF0F - interrupt flag (if is a C keyword)
} gb_interrupt_t;
```

`ime` lives in `gb_cpu_t` since it is not memory-mapped — it is an internal CPU flag toggled by `EI`/`DI`. Interrupt check occurs at the start of each `cpu_step` before fetch when `ime` is set.

---

## 9. Cartridge & MBC

The cartridge header is parsed on init to determine the MBC type. Each MBC type is implemented as a set of read/write handlers behind a common interface:

```c
typedef struct {
    uint8_t *rom;
    uint8_t *ram;
    uint32_t rom_size;
    uint32_t ram_size;
    gb_mbc_type_t mbc_type;
    // MBC state (bank registers etc)
} gb_cart_t;
```

MBC types to support in order of priority: ROM only, MBC1, MBC3 (with RTC), MBC5. These cover the vast majority of the commercial library.

---

## 10. Naming Conventions

- Types: `snake_case` with `_t` suffix — `gb_cpu_t`, `gb_register_t`
- Enums: type `snake_case_e`, members `UPPER_SNAKE_CASE` — `gb_opcode_uc_e`, `GB_OPCODE_UC_NOP`
- Functions: `component_verb_noun` — `cpu_step`, `mem_read`, `cart_init`
- Macros/constants: `UPPER_SNAKE_CASE` — `M_CYCLES_PER_FRAME`
- No global state — all state lives in `gb_t` and is passed by pointer
