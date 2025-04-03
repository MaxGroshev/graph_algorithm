cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(graph)

#-----------------------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TESTING_ENABLED ON CACHE BOOL [FORCE])
set(CMAKE_CPP_COMPILER clang)
add_compile_options(-std=c++23)

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address)
    add_compile_definitions(MY_DEBUG)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

add_subdirectory(graphviz_lib/)
add_subdirectory(debug_utils/) #make compile only in debug mode
add_subdirectory(algorythm/)
add_subdirectory(graph/)

add_subdirectory(./tests/)
if (TESTING_ENABLED)
endif()

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


