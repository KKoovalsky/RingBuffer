cmake_minimum_required(VERSION 3.16)

project(JunglesRingBuffer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(jungles_ring_buffer INTERFACE)
target_include_directories(jungles_ring_buffer INTERFACE ${CMAKE_CURRENT_LIST_DIR})
add_library(jungles::ring_buffer ALIAS jungles_ring_buffer)

option(JUNGLES_RING_BUFFER_ENABLE_TESTING "Enables self-testing of the library" OFF)
if(JUNGLES_RING_BUFFER_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
