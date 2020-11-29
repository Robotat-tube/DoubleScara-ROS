cmake_minimum_required(VERSION 2.8.3)
project(forward_kinematics)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg geometry_msgs)

## Declare ROS messages and services
#add_message_files(FILES Num.msg)

## Generate added messages and services
#generate_messages(DEPENDENCIES std_msgs geometry_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(forward_kinematics src/forward_kinematics.cpp)
target_link_libraries(forward_kinematics ${catkin_LIBRARIES})
# add_dependencies(forward_kinematics beginner_tutorials_generate_messages_cpp)

