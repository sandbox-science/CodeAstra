cmake_minimum_required(VERSION 3.16)

# Project name
set(TARGET_NAME CodeAstraApp)
set(EXECUTABLE_NAME CodeAstra)

set(QT_MAJOR_VERSION 6)

project(${TARGET_NAME} VERSION 0.0.1 DESCRIPTION "Code Editor written in C++ using Qt6")

# Enable automatic MOC (Meta-Object Compiler) handling for Qt
set(CMAKE_AUTOMOC ON)

# Set the CXX standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

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

# Find Qt components
find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Core Widgets Test)

# Locate yaml-cpp
if(APPLE)
    set(yaml-cpp_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0/lib/cmake/yaml-cpp")
endif()
find_package(yaml-cpp REQUIRED CONFIG)

# Copy YAML files to the build directory (non-macOS case)
set(YAML_SOURCE_DIR ${CMAKE_SOURCE_DIR}/config)
set(YAML_DEST_DIR ${CMAKE_BINARY_DIR}/config)
file(MAKE_DIRECTORY ${YAML_DEST_DIR})
file(GLOB YAML_FILES "${YAML_SOURCE_DIR}/*.yaml")

foreach(YAML_FILE ${YAML_FILES})
    file(COPY ${YAML_FILE} DESTINATION ${YAML_DEST_DIR})
endforeach()

# Create the CodeAstra library
add_library(${TARGET_NAME}
    src/MainWindow.cpp
    src/CodeEditor.cpp
    src/Tree.cpp
    src/FileManager.cpp
    src/Syntax.cpp
    src/SyntaxManager.cpp
    include/MainWindow.h
    include/CodeEditor.h
    include/Tree.h
    include/LineNumberArea.h
    include/FileManager.h
    include/syntaxManager.h
    include/syntax/Syntax.h
)

# Link YAML-CPP to the CodeAstra library
target_link_libraries(${TARGET_NAME} PRIVATE yaml-cpp)

# Create the executable for the application
add_executable(${EXECUTABLE_NAME} src/main.cpp)

# Ensure YAML config files are copied into macOS app bundle
# if(APPLE)
#     add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>/Contents/Resources/config"
#         COMMAND ${CMAKE_COMMAND} -E copy_directory "${YAML_SOURCE_DIR}" "$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>/Contents/Resources/config"
#         COMMENT "Copying YAML config files into macOS app bundle..."
#     )
# endif()

# Link the main executable with the CodeAstra library and Qt libraries
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${TARGET_NAME} Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets)

# Add the tests subdirectory
add_subdirectory(tests)

# Qt resource files
qt_add_resources(APP_RESOURCES resources.qrc)
target_sources(${EXECUTABLE_NAME} PRIVATE ${APP_RESOURCES})

# Compiler flags per OS
if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
elseif(APPLE)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    # set_target_properties(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${Qt${QT_MAJOR_VERSION}_INCLUDE_DIRS})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Set output names properly for Debug and Release
set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}" 
    DEBUG_OUTPUT_NAME "${EXECUTABLE_NAME}d" 
    RELEASE_OUTPUT_NAME ${EXECUTABLE_NAME}
)

# Link necessary Qt libraries to CodeAstra library
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets)

# Ensure correct linking of yaml-cpp (macOS)
if(APPLE)
    target_include_directories(${EXECUTABLE_NAME} PRIVATE /opt/homebrew/include)
    target_link_directories(${EXECUTABLE_NAME} PRIVATE /opt/homebrew/lib)
endif()