cmake_minimum_required(VERSION 3.5)

project(TaskTracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/server)
add_subdirectory(src/client)
add_subdirectory(src/utils)
