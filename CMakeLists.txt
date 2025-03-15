cmake_minimum_required(VERSION 3.16)

# Project name
set(TARGET_NAME CodeAstra)

set(QT_MAJOR_VERSION 6)

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "Code Editor written in C++ using Qt6")

# Enable automatic MOC (Meta-Object Compiler) handling for Qt
set(CMAKE_AUTOMOC ON)

# Set the CXX standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Detect operating system
if(WIN32)
    set(OS_NAME "Windows")
elseif(APPLE)
    set(OS_NAME "macOS")
else()
    set(OS_NAME "Linux")
endif()

message(STATUS "Building for ${OS_NAME}")

# Locate Qt installation
if(DEFINED ENV{Qt${QT_MAJOR_VERSION}_HOME})
    set(Qt_DIR "$ENV{Qt${QT_MAJOR_VERSION}_HOME}")
    message(STATUS "Using Qt from: ${Qt_DIR}")
else()
    if(WIN32)
        set(Qt_DIR "C:/Qt/${QT_MAJOR_VERSION}/msvc2022_64/lib/cmake/Qt${QT_MAJOR_VERSION}")
    elseif(APPLE)
        set(Qt_DIR "/usr/local/opt/qt/lib/cmake/Qt${QT_MAJOR_VERSION}")
    else()
        set(Qt_DIR "/usr/lib/cmake/Qt${QT_MAJOR_VERSION}")
    endif()
    message(STATUS "Using default Qt path: ${Qt_DIR}")
endif()

# Set Qt path for find_package
set(CMAKE_PREFIX_PATH ${Qt_DIR})

# Find Qt Widgets
find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets REQUIRED)

# Add executable and source files
add_executable(${TARGET_NAME}
    src/main.cpp
    src/MainWindow.cpp
    src/CodeEditor.cpp
    src/Syntax.cpp
    src/Tree.cpp
    include/MainWindow.h
    include/CodeEditor.h
    include/Syntax.h
    include/Tree.h
)

qt_add_resources(APP_RESOURCES resources.qrc)
target_sources(${TARGET_NAME} PRIVATE ${APP_RESOURCES})

# Compiler flags per OS
if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
elseif(APPLE)
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE ${Qt${QT_MAJOR_VERSION}_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Set output names properly for Debug and Release
set_target_properties(${TARGET_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}" 
    DEBUG_OUTPUT_NAME "${TARGET_NAME}d" 
    RELEASE_OUTPUT_NAME ${TARGET_NAME}
)

# Link necessary Qt libraries
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)