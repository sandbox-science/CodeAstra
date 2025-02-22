cmake_minimum_required(VERSION 3.16)
project(CodeAstra)

set(CMAKE_CXX_STANDARD 17)

# Automatically include current directory for includes and moc processing
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Automatically generate moc files for Q_OBJECT
set(CMAKE_AUTOMOC ON)

# Find the Qt6 Widgets library
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Add the executable with source and header files
add_executable(CodeAstra
    src/main.cpp
    src/mainwindow.cpp
    src/codeeditor.cpp
    include/mainwindow.h
    include/codeeditor.h
)

# Link the Qt6 Widgets library
target_link_libraries(CodeAstra Qt6::Widgets)