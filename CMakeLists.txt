cmake_minimum_required(VERSION 3.10)
project(catapult C)

add_compile_options(-Wall -Wextra)
add_definitions(-DCRC32_FAST)

set(SOURCES
    src/catapult.c
    src/pul.c
    src/binary.c
    src/crc32.c
)

add_executable(catapult ${SOURCES})
target_compile_definitions(catapult PRIVATE $<$<CONFIG:Debug>:DEBUG>)
