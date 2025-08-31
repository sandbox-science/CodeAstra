set(TARGET_NAME CodeAstraApp)
set(EXECUTABLE_NAME CodeAstra)

# Source files
set(SOURCES
    MainWindow.cpp
    CodeEditor.cpp
    Tree.cpp
    FileManager.cpp
    Syntax.cpp
    SyntaxManager.cpp
)

# Headers
set(HEADERS
    ${CMAKE_SOURCE_DIR}/include/MainWindow.h
    ${CMAKE_SOURCE_DIR}/include/CodeEditor.h
    ${CMAKE_SOURCE_DIR}/include/Tree.h
    ${CMAKE_SOURCE_DIR}/include/FileManager.h
    ${CMAKE_SOURCE_DIR}/include/Syntax.h
    ${CMAKE_SOURCE_DIR}/include/SyntaxManager.h
    ${CMAKE_SOURCE_DIR}/include/LineNumberArea.h
)

# Find yaml-cpp using CMake's package config
find_package(yaml-cpp REQUIRED)

# Library
add_library(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link against the proper target
target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Widgets yaml-cpp::yaml-cpp)
set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Executable
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${TARGET_NAME} Qt6::Core Qt6::Widgets)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Resources
qt_add_resources(APP_RESOURCES ${CMAKE_SOURCE_DIR}/resources.qrc)
target_sources(${EXECUTABLE_NAME} PRIVATE ${APP_RESOURCES})

# OS-specific flags
option(DEBUG "Enable debug mode" OFF)

if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX /analyze /sdl /guard:cf)
elseif(APPLE OR UNIX)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion -fsanitize=address,undefined -fstack-protector)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address,undefined)

    # Add the preprocessor symbol only when DEBUG is ON
    if (DEBUG)
        target_compile_definitions(${EXECUTABLE_NAME} PRIVATE DEBUG=1)
    endif()
endif()
