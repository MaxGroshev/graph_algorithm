cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(rb_tree)

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])
set(CMAKE_CPP_COMPILER clang)
add_compile_options(-std=c++20)

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address)
    add_compile_definitions(MY_DEBUG)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

add_subdirectory(graph_lib/)
add_subdirectory(debug_utils/) #make compile only in debug mode
add_subdirectory(rb_tree/)

add_subdirectory(./tests/)
if (TESTING_ENABLED)
endif()

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


