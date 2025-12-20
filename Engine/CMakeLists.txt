cmake_minimum_required(VERSION 3.20)
project (Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    SFML REQUIRED COMPONENTS 
    graphics
    network
    window 
    system
)

find_package(nlohmann_json REQUIRED)

file(GLOB_RECURSE SRC_CPP_FILES "src/*.cpp")
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
    sfml-graphics
    sfml-network
    sfml-window 
    sfml-system
    nlohmann_json::nlohmann_json
)