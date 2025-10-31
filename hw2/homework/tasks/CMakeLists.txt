cmake_minimum_required(VERSION 3.16)

set(OpenVINO_DIR "/opt/intel/openvino_2024.6.0/runtime/cmake")

find_package(OpenVINO REQUIRED COMPONENTS Runtime)

add_library(auto_aim OBJECT 
    armor.cpp
    yolo.cpp
    yolos/yolov5.cpp
)

target_link_libraries(auto_aim io openvino::runtime )