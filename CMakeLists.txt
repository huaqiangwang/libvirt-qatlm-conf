cmake_minimum_required(VERSION 3.16)
project(libvirt_qatlm_conf)

# Using C++17 for filesystem library
set(CMAKE_CXX_STANDARD 17)

set(TEST_NAME, "virtconftest")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(libvirt_qatlm_conf main.cpp ConfItem.cpp LibvirtConf.cpp)

enable_testing()
add_subdirectory(test)
add_test(NAME virtconf_test COMMAND virtconf_test)
