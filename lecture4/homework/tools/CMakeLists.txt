cmake_minimum_required(VERSION 3.16)
find_package(OpenCV REQUIRED)
add_library(tools OBJECT 
    img_tools.cpp
    logger.cpp
    plotter.cpp
)