cmake_minimum_required(VERSION 3.16.3)

project(Lecture_4)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main src/main.cpp tasks/detector.cpp)
target_link_libraries(main ${OpenCV_LIBS} fmt::fmt)
