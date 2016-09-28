cmake_minimum_required(VERSION 3.3)
project(ccatch C)

add_compile_options(
    $<$<COMPILE_LANGUAGE:C>:-std=c99>
    -Wall
    -Wpedantic
    -Werror
)

include_directories(include/)

enable_testing()
include(CTest)

add_executable(ccatch_test tests/ccatch_without_main.c tests/ccatch_test.c )
add_test(ccatch ccatch_test)
