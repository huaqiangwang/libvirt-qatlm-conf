cmake_minimum_required(VERSION 3.16)
project(libvirt_qatlm_conf)

set(CMAKE_CXX_STANDARD 14)
set(TEST_NAME, "virtconftest")

add_executable(libvirt_qatlm_conf main.cpp)

enable_testing()
add_subdirectory(test)
add_test(NAME virtconf_test COMMAND virtconf_test)
