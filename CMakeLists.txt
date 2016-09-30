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

install(DIRECTORY include/ DESTINATION include/ FILES_MATCHING PATTERN "*.h")

set(CPACK_GENERATOR TGZ DEB RPM)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION 0.1.0)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_CONTACT "Igor Sharonov <igsha@users.noreply.github.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Unit Testing Framework for pure C inspired by Catch for C++")
include(CPack)
