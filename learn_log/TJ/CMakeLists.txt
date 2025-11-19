cmake_minimum_required(VERSION 3.10)

project(learn_log_TJ CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Find dependencies
find_package(OpenCV 4.0 REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/lightbar.cpp
    src/img_tool.cpp
)

set(HEADERS
    include/lightbar.hpp
    include/img_tool.hpp
)

# Create executable
add_executable(learn_log_TJ ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(learn_log_TJ PRIVATE ${OpenCV_LIBS})

# Add compile options for better error messages
if(MSVC)
    target_compile_options(learn_log_TJ PRIVATE /W4)
else()
    target_compile_options(learn_log_TJ PRIVATE -Wall -Wextra -Wpedantic)
endif()
