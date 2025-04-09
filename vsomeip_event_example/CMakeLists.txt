cmake_minimum_required(VERSION 3.5)
project(vsomeip_speed_control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(vsomeip3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 서비스 실행파일
add_executable(service src/service.cpp)
target_link_libraries(service vsomeip3)


# 클라이언트 실행파일
add_executable(client src/client.cpp)
target_link_libraries(client vsomeip3)
