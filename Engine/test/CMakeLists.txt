cmake_minimum_required(VERSION 3.15)
project(CheckersEngineTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCE_FILES
    src/engine_tests.cpp
    main.cpp
)

add_executable(unitTest ${TEST_SOURCE_FILES})

target_include_directories(unitTest
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(unitTest
    PRIVATE
    gtest
    gmock
)

enable_testing()

include(GoogleTest)