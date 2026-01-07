cmake_minimum_required(VERSION 3.20)
project(utils VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(utils STATIC
    src/ThreadPool.cpp
    src/ThreadPoolManager.cpp
)

target_include_directories(utils
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS utils
    EXPORT utils-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(EXPORT utils-targets
    FILE utils-config.cmake
    NAMESPACE utils::
    DESTINATION lib/cmake/utils
)