cmake_minimum_required(VERSION 3.10)
project(Workspace_lecture2)

include_directories(part_01)
add_executable(test_1 part_01/test.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(test_2 part_02/TestForOpencv.cpp)
target_link_libraries(test_2 ${OpenCV_LIBS})

include_directories(part_03)
add_executable(test_3 part_03/main.cpp part_03/TraditionalArmor.cpp)
target_link_libraries(test_3 ${OpenCV_LIBS})