cmake_minimum_required(VERSION 3.10)

project(lirm-v2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/streamer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
