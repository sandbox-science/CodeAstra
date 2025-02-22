cmake_minimum_required(VERSION 3.16)

#Project name
set(TARGET_NAME CodeAstra)

#You can change this to Qt5 if you
#are not ready to migrate to Qt6.
set(QT_MAJOR_VERSION 6)

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "Code Editor written in C++ using Qt6")

#Make sure CMake will take care of moc for us
set(CMAKE_AUTOMOC ON)

#Set the CXX standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#See if the environment var is set
if(DEFINED ENV{Qt${QT_MAJOR_VERSION}_HOME})
    message(STATUS "Looking for Qt in: " $ENV{Qt${QT_MAJOR_VERSION}_HOME})
else()
    message(STATUS "Qt${QT_MAJOR_VERSION}_HOME environment variable not set. Checking default paths.")
endif()

#find_package should find everything fine so long as the ENV Variable is set or, for linux systems,
#it is in the default install path.
find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets REQUIRED PATHS $ENV{Qt${QT_MAJOR_VERSION}_HOME})

add_executable(${TARGET_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/codeeditor.cpp
    include/mainwindow.h
    include/codeeditor.h
)
    
if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${Qt${QT_MAJOR_VERSION}_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}" DEBUG_OUTPUT_NAME "${TARGET_NAME}d" RELEASE_OUTPUT_NAME ${TARGET_NAME})

#Add any extra libs to link also.
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)