cmake_minimum_required(VERSION 3.10)

project(streamstack)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

option(build_tests "build unit tests" ON)

find_package(OpenCV 4.10.0 EXACT REQUIRED PATHS /opt/opencv4.10.0/ NO_DEFAULT_PATH)
set(3rd_libs ${OpenCV_LIBS})
set(3rd_incs ${OpenCV_INCLUDE_DIRS})

add_library(${CMAKE_PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  ${OpenCV_LIBS}
)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${OpenCV_INCLUDE_DIRS}
)

add_subdirectory(test)
