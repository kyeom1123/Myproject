cmake_minimum_required(VERSION 3.5)
project(sensor_node)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

include_directories(include)

add_executable(sensor_node
  src/sensor_node.cpp
)

add_executable(temperature_subscriber
  src/temperature_subscriber.cpp
  src/main.cpp
)

ament_target_dependencies(sensor_node rclcpp sensor_msgs)
ament_target_dependencies(temperature_subscriber rclcpp sensor_msgs)

install(TARGETS
  sensor_node
  temperature_subscriber
  DESTINATION lib/${PROJECT_NAME})

ament_package()
