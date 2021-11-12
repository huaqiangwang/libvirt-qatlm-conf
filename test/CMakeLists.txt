message("Test: "  ${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}
        /usr/local/include
        /usr/include
)

link_directories(
        /usr/local/lib
        /usr/lib
)

add_executable(${TEST_NAME} testConfItem.cpp)
TARGET_LINK_LIBRARIES(${TEST_NAME} gtest gtest_main )
