cmake_minimum_required(VERSION 3.16)
project(vsomeip_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

# vsomeip는 보통 /usr/local 경로에 설치됨
find_path(VSOMEIP_INCLUDE_DIR vsomeip/vsomeip.hpp)
find_library(VSOMEIP_LIB vsomeip3)

if(NOT VSOMEIP_INCLUDE_DIR OR NOT VSOMEIP_LIB)
  message(FATAL_ERROR "vSomeIP headers or library not found. Install vSomeIP first.")
endif()

include_directories(${VSOMEIP_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_executable(someip_server src/server.cpp)
target_link_libraries(someip_server ${VSOMEIP_LIB} ${Boost_LIBRARIES} Threads::Threads)

add_executable(someip_client src/client.cpp)
target_link_libraries(someip_client ${VSOMEIP_LIB} ${Boost_LIBRARIES} Threads::Threads)
