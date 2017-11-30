cmake_minimum_required(VERSION 3.8)
project(splunk_hec_client_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(["../"])

set(LIB_SOURCE_FILES
        LICENSE
        README.md event.h serialize.h raw_event.h json_event.h)
set(JSON_EVENT_EXAMPLE ${LIB_SOURCE_FILES} json_event_example.cpp)

add_executable(${JSON_EVENT_EXAMPLE})