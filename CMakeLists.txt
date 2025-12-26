cmake_minimum_required(VERSION 4.0)
project(IstanbulMetroRotaPlanlayici C)

set(CMAKE_C_STANDARD 11)


include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
)


FetchContent_MakeAvailable(raylib)


add_executable(IstanbulMetroRotaPlanlayici
        main.c
        src/route_solver.c
        src/min_heap.c
        src/bst_tree.c
        src/graph_loader.c
        src/ui.c
)


target_link_libraries(IstanbulMetroRotaPlanlayici PRIVATE raylib)

if (WIN32)
    target_link_libraries(IstanbulMetroRotaPlanlayici PRIVATE winmm gdi32)
endif()