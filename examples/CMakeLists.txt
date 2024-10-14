cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB EXAMPLE_SOURCES "*.cpp")

foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE} JAF)
endforeach()
