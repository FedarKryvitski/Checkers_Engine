cmake_minimum_required(VERSION 3.20)
project(Engine LANGUAGES CXX)

# Only for Windows
enable_language(ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)

add_subdirectory(utils)

SET(SRC_CPP_FILES 
    "src/engine.cpp"
    "src/gamecontroller.cpp"
    "src/moves.cpp"
    "src/main.cpp"
)
file(GLOB_RECURSE SRC_ASM_FILES "src/*.asm")

add_executable(
    ${PROJECT_NAME}
    ${SRC_CPP_FILES}
    ${SRC_ASM_FILES}
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    include
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    spdlog::spdlog
    utils
)

enable_testing()
#add_subdirectory(test)