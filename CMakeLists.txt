cmake_minimum_required(VERSION 3.15)
project(JAF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/vendor/SDL")

include_directories(${PROJECT_NAME} "include")
include_directories(${PROJECT_NAME} "vendor/stb")
include_directories(${PROJECT_NAME} "${SDL2_DIR}/include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

find_library(SDL2_LIBRARY SDL2-static PATHS "${SDL2_DIR}/lib")
find_library(SDL2_MAIN_LIBRARY SDL2main PATHS "${SDL2_DIR}/lib")

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_MAIN_LIBRARY}
        winmm
        imm32
        version
        setupapi
        cfgmgr32
)

add_subdirectory(examples)