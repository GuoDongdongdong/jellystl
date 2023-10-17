cmake_minimum_required(VERSION 3.16.3)

project(
    jellystl
    LANGUAGES CXX
    VERSION 0.1.0
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE include)

add_subdirectory(test)
