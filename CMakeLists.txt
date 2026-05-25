cmake_minimum_required(VERSION 3.10)
project(catapult C)

set(SOURCES
    src/catapult.c
    src/pul.c
    src/binary.c
    src/crc32.c
)

if(UNIX)
    add_compile_options(-Wall -Wextra)
elseif(WIN32)
    list(APPEND SOURCES src/getopt/getopt.c)
    include_directories(src/getopt)
endif()

add_definitions(-DCRC32_FAST)
add_executable(catapult ${SOURCES})
target_compile_definitions(catapult PRIVATE $<$<CONFIG:Debug>:DEBUG>)
