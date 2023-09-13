cmake_minimum_required(VERSION 3.8)
project(robo_cruise_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)

add_executable(robo_cruise_controller_node src/controller.cpp)
target_include_directories(robo_cruise_controller_node PRIVATE include)
ament_target_dependencies(robo_cruise_controller_node rclcpp geometry_msgs trajectory_msgs)

install(TARGETS
        robo_cruise_controller_node
        DESTINATION lib/${PROJECT_NAME})

ament_package()
