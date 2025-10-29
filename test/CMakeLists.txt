cmake_minimum_required(VERSION 3.10)
project(VectorTests)

find_package(GTest REQUIRED)

add_executable(run_tests test.cpp ../vector.h)

target_link_libraries(run_tests GTest::gtest GTest::gtest_main pthread)

target_compile_features(run_tests PRIVATE cxx_std_14)