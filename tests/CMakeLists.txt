enable_testing()

find_package(yaml-cpp REQUIRED CONFIG)

# Add test executables
add_executable(test_mainwindow test_mainwindow.cpp)
add_executable(test_filemanager test_filemanager.cpp)
add_executable(test_syntax test_syntax.cpp)

# Link libraries
foreach(test_target IN ITEMS test_mainwindow test_filemanager test_syntax)
    target_link_libraries(${test_target} PRIVATE
        ${EXECUTABLE_NAME}
        Qt6::Widgets
        Qt6::Test
        yaml-cpp::yaml-cpp
    )
    target_include_directories(${test_target} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/tests
    )
    set_property(SOURCE ${test_target}.cpp PROPERTY SKIP_AUTOMOC OFF)

    add_test(NAME ${test_target} COMMAND ${test_target})
endforeach()
