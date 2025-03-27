cmake_minimum_required(VERSION 3.10)
project(MyProject)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 소스 파일 목록
set(SOURCES
    src/main.cpp
    src/Vehicle.cpp
    src/ElectricCar.cpp
)

# 실행 파일 생성
add_executable(my_program ${SOURCES})

# 출력 경로 설정 (선택 사항)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
