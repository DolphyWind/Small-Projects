cmake_minimum_required(VERSION 3.25)
project(one_dim_keyboard)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BINARY_NAME odkb)

find_package(SQLiteCpp REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/util.cpp
)

add_executable(${BINARY_NAME} ${SOURCE_FILES})
target_include_directories(${BINARY_NAME} PRIVATE include)
target_link_libraries(${BINARY_NAME} PRIVATE SQLiteCpp)
