cmake_minimum_required(VERSION 3.18.0)
project(ArmorDetector 
    VERSION 0.1.0 
    LANGUAGES CXX
)
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please install OpenCV first.")
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(ArmorDetector
    ArmorDetector.cpp
    LightBar.cpp
    ToolUtils.cpp
    main.cpp
)
target_link_libraries(ArmorDetector
    PRIVATE ${OpenCV_LIBS}
)