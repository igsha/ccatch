cmake_minimum_required(VERSION 3.3)
project(ccatch VERSION 0.1.0 LANGUAGES C)

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

install(DIRECTORY include/ DESTINATION include/ COMPONENT Devel FILES_MATCHING PATTERN "*.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/CCatchConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES CCatchConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/CCatchConfigVersion.cmake
    DESTINATION share/cmake COMPONENT Devel
)

set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_CONTACT "Igor Sharonov <igsha@users.noreply.github.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Unit Testing Framework for pure C inspired by Catch for C++")
include(CPack)
