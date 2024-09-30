cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)

# Подключаем Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# find_package(GTest CONFIG REQUIRED)
enable_testing()

add_executable(unit_test
        ib01ad.test.cpp
)

target_link_libraries(unit_test PRIVATE slicotpp slicot GTest::gmock GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(
        unit_test
)