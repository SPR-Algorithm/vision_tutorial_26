# 1. 指定CMake最低版本
cmake_minimum_required(VERSION 3.18.0)

# 2. 项目名称
project(ArmorDetector 
    VERSION 0.1.0 
    LANGUAGES CXX
)

# 3. 查找OpenCV依赖
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please install OpenCV first.")
endif()

# 4. 包含当前目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 5. 定义可执行文件 
add_executable(ArmorDetector
    ArmorDetector.cpp
    LightBar.cpp
    ToolUtils.cpp
    main.cpp
)

# 6. 链接OpenCV库（将你的可执行文件与OpenCV绑定）
target_link_libraries(ArmorDetector
    PRIVATE ${OpenCV_LIBS}
)



