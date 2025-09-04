cmake_minimum_required(VERSION 3.16)
project(vsomeip_db_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)

find_path(VSOMEIP_INCLUDE_DIR vsomeip/vsomeip.hpp)
find_library(VSOMEIP_LIB vsomeip3)

if(NOT VSOMEIP_INCLUDE_DIR OR NOT VSOMEIP_LIB)
  message(FATAL_ERROR "vSomeIP headers or library not found. Please install vSomeIP first.")
endif()

include_directories(${VSOMEIP_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} include)

add_executable(someip_server src/server.cpp src/db.cpp)
target_link_libraries(someip_server ${VSOMEIP_LIB} ${Boost_LIBRARIES} Threads::Threads)

add_executable(someip_client src/client.cpp src/db.cpp)
target_link_libraries(someip_client ${VSOMEIP_LIB} ${Boost_LIBRARIES} Threads::Threads)

# Ensure runtime data is available when running from the build directory
add_custom_command(TARGET someip_server POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/db
          ${CMAKE_BINARY_DIR}/db)
add_custom_command(TARGET someip_client POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/db
          ${CMAKE_BINARY_DIR}/db)
