cmake_minimum_required(VERSION 4.0)
project(IstanbulMetroRotaPlanlayici C)

set(CMAKE_C_STANDARD 11)

add_executable(IstanbulMetroRotaPlanlayici main.c
        src/route_solver.c
        src/min_heap.c
        src/bst_tree.c
        src/graph_loader.c)
