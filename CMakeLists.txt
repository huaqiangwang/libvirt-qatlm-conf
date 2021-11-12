cmake_minimum_required(VERSION 3.19)
project(libvirt_qatlm_conf)

set(CMAKE_CXX_STANDARD 14)
set(TEST_NAME, "virtconf-test")

add_executable(libvirt_qatlm_conf main.cpp)

enable_testing()
add_subdirectory(test)
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
