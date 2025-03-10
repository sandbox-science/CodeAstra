# Enable testing
enable_testing()

# Add individual test executables
add_executable(test_mainwindow
    test_mainwindow.cpp
    ../src/MainWindow.cpp
    ../src/CodeEditor.cpp
    ../src/Syntax.cpp
    ../include/MainWindow.h
    ../include/CodeEditor.h
    ../include/Syntax.h
)

# Link each test with necessary libraries
target_link_libraries(test_mainwindow PRIVATE Qt6::Widgets Qt6::Test)

# Register each test with CTest
add_test(NAME test_mainwindow COMMAND test_mainwindow)

# Set the runtime output directory for the test executables
set_target_properties(test_mainwindow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/tests
)