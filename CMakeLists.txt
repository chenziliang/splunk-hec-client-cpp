cmake_minimum_required(VERSION 3.8)
project(splunk_hec_client_cpp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        LICENSE
        README.md event.h serialize.h raw_event.h json_event.h)

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(examples)