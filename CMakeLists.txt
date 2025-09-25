cmake_minimum_required(VERSION 4.1.1)
project(snake-cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

add_executable(snake-cpp src/main.cpp src/snake.cpp)

target_link_libraries(snake-cpp PUBLIC SDL3::SDL3)
target_include_directories(snake-cpp PUBLIC ${SDL_INCLUDE_DIRS})
