# Enable testing
enable_testing()
find_package(yaml-cpp REQUIRED)
# Add individual test executables
add_executable(test_mainwindow test_mainwindow.cpp)
add_executable(test_tree test_tree.cpp)
add_executable(test_syntax test_syntax.cpp)

# Make sure the test executables link with the necessary libraries
target_link_libraries(test_mainwindow PRIVATE ${TARGET_NAME} Qt6::Widgets Qt6::Test yaml-cpp)
target_link_libraries(test_tree PRIVATE ${TARGET_NAME} Qt6::Widgets Qt6::Test yaml-cpp)
target_link_libraries(test_syntax PRIVATE ${TARGET_NAME} Qt6::Widgets Qt6::Test yaml-cpp)

# Register each test with CTest
add_test(NAME test_mainwindow COMMAND test_mainwindow)
add_test(NAME test_tree COMMAND test_tree)
add_test(NAME test_syntax COMMAND test_tree)

# Set the runtime output directory for the test executables
set_target_properties(test_mainwindow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/tests
)
set_target_properties(test_tree PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/tests
)
set_target_properties(test_syntax PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/tests
)

set_property(SOURCE test_mainwindow.cpp PROPERTY SKIP_AUTOMOC OFF)
set_property(SOURCE test_tree.cpp PROPERTY SKIP_AUTOMOC OFF)
set_property(SOURCE test_syntax.cpp PROPERTY SKIP_AUTOMOC OFF)

# Include directories for tests
target_include_directories(test_mainwindow PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_tree PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_syntax PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Ensure proper linking directories are set for yaml-cpp
target_include_directories(test_mainwindow PRIVATE /opt/homebrew/include)
target_include_directories(test_tree PRIVATE /opt/homebrew/include)
target_include_directories(test_syntax PRIVATE /opt/homebrew/include)

target_link_directories(test_mainwindow PRIVATE /opt/homebrew/lib)
target_link_directories(test_tree PRIVATE /opt/homebrew/lib)
target_link_directories(test_syntax PRIVATE /opt/homebrew/lib)

# Link the libraries explicitly for both main executable and tests
target_link_libraries(test_syntax PRIVATE /opt/homebrew/lib/libyaml-cpp.dylib)
