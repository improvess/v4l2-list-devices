cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME list-devices)
project(${PROJECT_NAME} CXX)

if (NOT CMAKE_BUILD_TYPE) 
  set(CMAKE_BUILD_TYPE Release) 
endif() 

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

link_directories(${CMAKE_PREFIX_PATH}/lib/)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -O3)

target_link_libraries(${PROJECT_NAME} v4l2)
