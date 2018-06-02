cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(test_my_big
        big_integer.cpp
        big_integer.h
               test_my_big.cpp )

add_executable(my_big_integer_testing
        big_integer_testing.cpp
        big_integer.cpp
        big_integer.h
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc my_vector.h my_vector.cpp)


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(test_my_big -lgmp -lgmpxx -lpthread)
target_link_libraries(my_big_integer_testing -lgmp -lgmpxx -lpthread)