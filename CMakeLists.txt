cmake_minimum_required(VERSION 3.15)
project(AStarGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags (MSVC compatible)
if(MSVC)
    set(CMAKE_CXX_FLAGS "/W3")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find SFML 3.0 (try with Graphics only first)
set(CMAKE_PREFIX_PATH "C:/sfml/SFML-3.0.2")
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics)

# Source files
set(SOURCES
    src/main.cpp
    src/grid.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries (try just Graphics first)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)

# Copy SFML DLLs to build directory on Windows
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/sfml/SFML-3.0.2/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()