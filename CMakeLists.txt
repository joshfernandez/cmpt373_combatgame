cmake_minimum_required(VERSION 3.5.1)
project(Adventure2016)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/lib/yaml-cpp/include
        ${CMAKE_SOURCE_DIR}/lib/expr-evaluator
        ${CMAKE_SOURCE_DIR}/tests/lib/googlemock/include
        ${CMAKE_SOURCE_DIR}/tests/lib/googletest/include
        ${CMAKE_SOURCE_DIR}/lib/ascii-converter
)

link_directories(${LIBRARY_OUTPUT_PATH})

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)
