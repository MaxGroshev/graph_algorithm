cmake_minimum_required(VERSION 3.28)

#-----------------------------------------------------------------------------------------

project(graph_alg)

#-----------------------------------------------------------------------------------------

add_library(${PROJECT_NAME}) 

target_sources(${PROJECT_NAME}
    PUBLIC
		FILE_SET CXX_MODULES     # Declare module interface files
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		FILES graph_alg_impl.cppm
)
