cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(graph)

#-----------------------------------------------------------------------------------------

set(SOURCE
    ./ui/ui.cpp)

#-----------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCE} ${DEBUG_UTILS} ./main.cpp)
target_link_libraries(${PROJECT_NAME} graphviz debug_utils graph_alg)
target_include_directories(${PROJECT_NAME} PRIVATE ./ui ./include/)



