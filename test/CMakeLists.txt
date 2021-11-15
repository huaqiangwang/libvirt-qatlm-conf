message("Test: "  ${CMAKE_CURRENT_SOURCE_DIR})

# Can 'TEST_NAME' be defined in root folder and used here?
set(TEST_NAME, "virtconftest")

include_directories(
        ${PROJECT_SOURCE_DIR}
        /usr/local/include
        /usr/include
)

link_directories(
        /usr/local/lib
        /usr/lib
)


add_executable(virtconf_test testConfItem.cpp)

TARGET_LINK_LIBRARIES(virtconf_test
	gtest
	gtest_main
	-pthread)
