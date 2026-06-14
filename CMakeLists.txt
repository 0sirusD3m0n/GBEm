cmake_minimum_required(VERSION 3.28.3)
project(gbem VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SHARED_SOURCES
    src/gbem/cart/cart.c
    src/gbem/cpu/handlers/alu.c
    src/gbem/cpu/handlers/bit.c
    src/gbem/cpu/handlers/control.c
    src/gbem/cpu/handlers/load.c
    src/gbem/cpu/handlers/misc.c
    src/gbem/cpu/cpu.c
    src/gbem/memory/bus.c
    src/gbem/memory/ram.c
    src/gbem/gb.c
    )
    

include(FetchContent)
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG v1.7.19
    )
FetchContent_MakeAvailable(cjson)

add_executable(gbem
    src/main.c
    ${SHARED_SOURCES}
)

add_executable(gbem_tests
    tests/main.c
    ${SHARED_SOURCES}
)

target_include_directories(gbem PRIVATE src/)
target_include_directories(gbem_tests PRIVATE 
    tests/ 
    src/
    ${cjson_SOURCE_DIR}
)
target_link_libraries(gbem_tests PRIVATE cjson)

target_compile_definitions(gbem_tests PRIVATE TESTING TEST_DATA_PATH="${CMAKE_SOURCE_DIR}/tests/data/sst/sm83/v1/")