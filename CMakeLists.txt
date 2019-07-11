cmake_minimum_required(VERSION 3.14)
project(reallyAsyncWithPredicate)

set(CMAKE_CXX_STANDARD 14)

add_executable(reallyAsyncWithPredicate main.cpp)
target_link_libraries(reallyAsyncWithPredicate pthread)