cmake_minimum_required(VERSION 3.8)
project(splunk_hec_client_cpp)

set(CMAKE_CXX_STANDARD 14)

set(JSON_EVENT_EXAMPLE json_event_example.cpp)
set(RAW_EVENT_EXAMPLE raw_event_example.cpp)
set(RAW_EVENT_BATCH_EXAMPLE raw_event_batch_example.cpp ../raw_event_batch.cpp)
set(JSON_EVENT_BATCH_EXAMPLE json_event_batch_example.cpp ../json_event_batch.cpp)

add_executable(json_event_example ${JSON_EVENT_EXAMPLE})
add_executable(raw_event_example ${RAW_EVENT_EXAMPLE})
add_executable(json_event_batch_example ${JSON_EVENT_BATCH_EXAMPLE})
add_executable(raw_event_batch_example ${RAW_EVENT_BATCH_EXAMPLE})
