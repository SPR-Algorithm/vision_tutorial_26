cmake_minimum_required(VERSION 3.10)
project(final)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

add_executable(final final.cpp)

target_link_libraries(final ${OpenCV_LIBS})
