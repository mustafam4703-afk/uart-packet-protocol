cmake_minimum_required(VERSION 3.14)
project(uart_protocol_tests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    test/test_native/test_encoder.cpp
    test/test_native/test_decoder.cpp
    lib/Protocol/PacketEncoder.cpp
    lib/Protocol/PacketDecoder.cpp
)

target_include_directories(tests PRIVATE lib/Protocol)

target_link_libraries(tests GTest::gtest_main GTest::gmock_main)