cmake_minimum_required (VERSION 3.8)

project(test)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 3 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(CTest)
include(Catch)

add_executable(vector_test vector_test.cpp)
add_executable(list_test list_test.cpp)
add_executable(deque_test deque_test.cpp)
add_executable(stack_test stack_test.cpp)
add_executable(queue_test queue_test.cpp)
add_executable(priority_queue_test priority_queue_test.cpp)

target_link_libraries(vector_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(vector_test)
target_link_libraries(list_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(list_test)
target_link_libraries(deque_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(deque_test)
target_link_libraries(stack_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(stack_test)
target_link_libraries(queue_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(queue_test)
target_link_libraries(priority_queue_test PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(priority_queue_test)
