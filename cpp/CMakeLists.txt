cmake_minimum_required(VERSION 3.7)
project(cpp)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_VERBOSE_MAKEFILE 1)

find_package(OpenMP REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/icecream)

add_executable(main_continuous_coop_prob.out main_continuous_coop_prob.cpp ContinuousStrategy.hpp ContinuousGame.hpp)
if(OpenMP_CXX_FOUND)
    target_link_libraries(main_continuous_coop_prob.out PUBLIC OpenMP::OpenMP_CXX)
endif()