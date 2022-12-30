cmake_minimum_required(VERSION 3.24)
project(deduper)

set(CMAKE_CXX_STANDARD 23)
set(SOURCE_FILES main.cpp)

add_executable(deduper ${SOURCE_FILES})

add_subdirectory(Google_tests)

target_link_libraries(deduper FileChunker_lib)

include_directories(FileChunker_lib)
add_subdirectory(FileChunker_lib)

