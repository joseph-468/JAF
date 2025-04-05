cmake_minimum_required(VERSION 3.15)
project(JAF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/vendor/raylib")
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/vendor/SDL2")
set(SPDLOG_DIR "${CMAKE_SOURCE_DIR}/vendor/spdlog")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

find_library(RAYLIB raylib PATHS "${RAYLIB_DIR}/lib" REQUIRED)
find_library(SDL2 sdl2 PATHS "${SDL2_DIR}/lib/x64" REQUIRED)
find_library(SPDLOG spdlog PATHS "${SPDLOG_DIR}/lib" REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
        "include"
        "${RAYLIB_DIR}/include"
        "${SDL2_DIR}/include"
        "${SPDLOG_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
        ${RAYLIB}
        ${SDL2}
        ${SPDLOG}
)
