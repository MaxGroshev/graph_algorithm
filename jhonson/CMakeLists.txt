cmake_minimum_required(VERSION 3.28)

#-----------------------------------------------------------------------------------------

project(graph)

#-----------------------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(TESTING_ENABLED ON CACHE BOOL [FORCE])
set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++-17")
set(CMAKE_CXX_STANDARD 23)
add_compile_options(-std=c++23 -fmodules)


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
add_subdirectory(tests/)

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


