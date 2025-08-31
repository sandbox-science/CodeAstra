cmake_minimum_required(VERSION 3.16)

project(CodeAstra VERSION 0.1.0 DESCRIPTION "Code Editor written in modern C++ using Qt6")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use cmake/ for custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable Qt tools
set(CMAKE_AUTOMOC ON)

# Define target names
set(TARGET_NAME CodeAstra)
set(EXECUTABLE_NAME ${TARGET_NAME}App)

# Set Qt version
set(QT_MAJOR_VERSION 6)

# Find Qt
find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Core Widgets Test)

# yaml-cpp
find_package(yaml-cpp REQUIRED CONFIG)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
