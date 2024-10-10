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
        read_identification_data.hpp
        read_identification_data.cpp
        f_matrix.test.cpp
        ib01ad.test.cpp
        ib01bd.test.cpp
        ib01.tools.hpp
        ib01.tools.cpp
        ib01bd_ws.f90
)

target_link_libraries(unit_test PRIVATE slicotpp GTest::gmock GTest::gmock_main)
target_compile_definitions(unit_test PRIVATE "DATA_PATH=${CMAKE_CURRENT_SOURCE_DIR}")

include(GoogleTest)
gtest_discover_tests(
        unit_test
)