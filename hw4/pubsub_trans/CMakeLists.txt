cmake_minimum_required(VERSION 3.8)
project(pubsub_trans)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)


add_executable(celsius_pub src/publisher_celsius_function.cpp)
target_link_libraries(celsius_pub PUBLIC rclcpp::rclcpp ${std_msgs_TARGETS})
add_executable(fahrenheit_sub src/subscriber_fahrenheit_function.cpp)
target_link_libraries(fahrenheit_sub PUBLIC rclcpp::rclcpp ${std_msgs_TARGETS})

install(TARGETS
  celsius_pub
  fahrenheit_sub
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
ament_package()
