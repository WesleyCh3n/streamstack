cmake_minimum_required(VERSION 3.10)

project(lirm-v2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

option(build_tests "build unit tests" ON)

find_package(OpenCV 4.10.0 EXACT REQUIRED PATHS /opt/opencv4.10.0/ NO_DEFAULT_PATH)

add_executable(${CMAKE_PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.cpp
)
set(3rd_libs ${OpenCV_LIBS})
set(3rd_incs ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${3rd_libs})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${3rd_incs})

add_subdirectory(test)
