cmake_minimum_required(VERSION 3.28)

#-----------------------------------------------------------------------------------------

project(graphviz)

#-----------------------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE .)

target_include_directories(${PROJECT_NAME} INTERFACE .)




