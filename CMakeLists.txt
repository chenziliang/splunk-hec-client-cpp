cmake_minimum_required(VERSION 3.8)
project(splunk_hec_client_cpp)

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/hec_exception.h ${PROJECT_SOURCE_DIR}/hec_exception.cpp ${PROJECT_SOURCE_DIR}/http_client_factory.h
        ${PROJECT_SOURCE_DIR}/serialize.h ${PROJECT_SOURCE_DIR}/event_inf.h ${PROJECT_SOURCE_DIR}/event.h
        ${PROJECT_SOURCE_DIR}/raw_event.h ${PROJECT_SOURCE_DIR}/json_event.h
        ${PROJECT_SOURCE_DIR}/event_batch.h ${PROJECT_SOURCE_DIR}/event_batch.cpp
        ${PROJECT_SOURCE_DIR}/json_event_batch.h ${PROJECT_SOURCE_DIR}/json_event_batch.cpp
        ${PROJECT_SOURCE_DIR}/raw_event_batch.h ${PROJECT_SOURCE_DIR}/raw_event_batch.cpp
        ${PROJECT_SOURCE_DIR}/indexer_inf.h ${PROJECT_SOURCE_DIR}/indexer.h ${PROJECT_SOURCE_DIR}/indexer.cpp
        ${PROJECT_SOURCE_DIR}/poller_inf.h ${PROJECT_SOURCE_DIR}/poller_callback_inf.h
        ${PROJECT_SOURCE_DIR}/http_response_poller.h ${PROJECT_SOURCE_DIR}/http_response_poller.cpp
        ${PROJECT_SOURCE_DIR}/hec_response_parser.h ${PROJECT_SOURCE_DIR}/ack_poller.h ${PROJECT_SOURCE_DIR}/ack_poller.cpp
        ${PROJECT_SOURCE_DIR}/load_balancer_inf.h ${PROJECT_SOURCE_DIR}/load_balancer.h ${PROJECT_SOURCE_DIR}/load_balancer.cpp
        ${PROJECT_SOURCE_DIR}/hec_inf.h ${PROJECT_SOURCE_DIR}/hec.h ${PROJECT_SOURCE_DIR}/hec.cpp
        ${PROJECT_SOURCE_DIR}/concurrent_hec.h ${PROJECT_SOURCE_DIR}/concurrent_hec.cpp ${PROJECT_SOURCE_DIR}/config.h
        )

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(examples)