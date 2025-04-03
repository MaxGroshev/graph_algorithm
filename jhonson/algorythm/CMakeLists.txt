cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(graph_alg)

#-----------------------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
    ${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)


