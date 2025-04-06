cmake_minimum_required (VERSION 3.28)

#-----------------------------------------------------------------------------------------

project (tests)

#-----------------------------------------------------------------------------------------

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#-----------------------------------------------------------------------------------------

#Unit_tests
set (UNIT_TESTS
    ./unit_tests/main.cpp)


#-----------------------------------------------------------------------------------------

add_executable            (unit_tests ${UNIT_TESTS} ${HEADERS} ${DEBUG_UTILS})
target_include_directories(unit_tests PRIVATE${GTEST_INCLUDE_DIRS}
../graph/include/ ../graph/ui/)
target_link_libraries     (unit_tests ${GTEST_BOTH_LIBRARIES} graphviz debug_utils graph_alg)


