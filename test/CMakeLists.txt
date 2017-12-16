cmake_minimum_required(VERSION 3.8)
project(splunk_hec_client_cpp)

set(CMAKE_CXX_STANDARD 14)
include_directories(${OPENSSL_ROOT_DIR}/include)
set(UNITTEST unit_test_main.cpp json_event_test.cpp)

add_executable(unit_test_main ${UNITTEST})

link_directories(${OPENSSL_ROOT_DIR}/lib)

if(WIN32)
endif(WIN32)

if(APPLE)
    set(DEPENDENT_LIBS cpprest boost_system-mt boost_thread-mt boost_chrono-mt ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib ${OPENSSL_ROOT_DIR}/lib/libssl.dylib)
endif(APPLE)

if(UNIX AND NOT APPLE)
    set(DEPENDENT_LIBS cpprest boost_system boost_thread boost_chrono pthread crypto ssl)
endif(UNIX AND NOT APPLE)

target_link_libraries(unit_test_main ${DEPENDENT_LIBS})
