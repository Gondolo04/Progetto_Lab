cmake_minimum_required(VERSION 3.20)
project(AStarGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Use modern CMake target-based configuration
if(MSVC)
    # Add warning level but don't override standard library settings
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Find SFML 3.0 (try with Graphics only first)
set(CMAKE_PREFIX_PATH "C:/sfml/SFML-3.0.2")
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics)

# Enable testing
enable_testing()


# Google Test setup
include(FetchContent)

# Set policy for download timestamp
cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create a library for the Grid class (so it can be tested)
add_library(pathfinding_lib STATIC
    src/grid.cpp
    src/character.cpp
)

# Set C++ standard for the library
target_compile_features(pathfinding_lib PUBLIC cxx_std_17)

target_include_directories(pathfinding_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(pathfinding_lib PUBLIC SFML::Graphics)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE pathfinding_lib)

# Copy SFML DLLs to build directory on Windows
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/sfml/SFML-3.0.2/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# Tests
add_executable(grid_tests tests/grid_test.cpp)
target_compile_features(grid_tests PRIVATE cxx_std_17)
target_link_libraries(grid_tests 
    pathfinding_lib 
    gtest_main 
    gtest
)

include(GoogleTest)
gtest_discover_tests(grid_tests)