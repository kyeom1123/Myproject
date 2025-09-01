cmake_minimum_required(VERSION 3.15)
project(EduCppProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 사용자 컴파일 옵션 추가 (예시: -Wall -O2)
set(USER_COMPILE_OPTIONS "-Wall" CACHE STRING "사용자 정의 컴파일 옵션")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main src/main.cpp)
target_compile_options(main PRIVATE ${USER_COMPILE_OPTIONS})