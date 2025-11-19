cmake_minimum_required(VERSION 3.14)
project(PmrStackProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    include/fixed_block_resource.hpp
    include/pmr_stack.hpp
)

set(SOURCES
    src/fixed_block_resource.cpp
)

add_executable(main
    src/main.cpp
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(tests
    test/tests.cpp
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests PRIVATE gtest gtest_main)