cmake_minimum_required(VERSION 3.22.1)
project(task1)

set(CMAKE_CXX_STANDARD 17)

add_library(graphs_lib STATIC
        module3/task1/graphs_lib/igraph.hpp
        module3/task1/graphs_lib/matrixgraph.hpp
        module3/task1/graphs_lib/listgraph.hpp
        module3/task1/graphs_lib/setgraph.hpp
        module3/task1/graphs_lib/arcgraph.hpp
)

target_include_directories(graphs_lib PUBLIC module3/task1/graphs_lib)

set_target_properties(graphs_lib PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
        task1
        module3/task1/task1.cpp
)

target_link_libraries(
        task1
        PRIVATE graphs_lib
)
