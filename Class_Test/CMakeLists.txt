cmake_minimum_required(VERSION 3.5)
project(vsomeip_speed_control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(vsomeip3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# class_test 실행파일
add_executable(class_test src/class_test.cpp)

# class_test 실행파일
add_executable(class_test2 src/class_test2.cpp)
