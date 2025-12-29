cmake_minimum_required(VERSION 3.15)

# Eski CMake politikalarıyla uyumluluk (Raylib için gerekli)
# Bu değişken, alt projelerin eski CMake sürümlerini kullanmasına izin verir
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum CMake policy version")

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(IstanbulMetroRotaPlanlayici C)

# C standardı
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ==================== Kaynak Dosyalar ====================
set(SOURCES
    main.c
    src/route_solver.c
    src/min_heap.c
    src/bst_tree.c
    src/graph_loader.c
    src/ui.c
)

set(HEADERS
    includes/structs.h
    includes/algorithms.h
    includes/ui.h
)

# ==================== Raylib Bağımlılığı ====================
include(FetchContent)

# Raylib seçenekleri
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5  # En güncel kararlı sürüm
)
FetchContent_MakeAvailable(raylib)

# ==================== Yürütülebilir Dosya ====================
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include dizinleri (hem CLion hem VS Code için)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# ==================== Bağlantı Kütüphaneleri ====================
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Windows için ek kütüphaneler
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32)
endif()

# ==================== Derleme Ayarları ====================
# Debug modda uyarıları etkinleştir
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# Veri dosyalarını build dizinine kopyala
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/metro.csv 
               ${CMAKE_CURRENT_BINARY_DIR}/metro.csv COPYONLY)

# ==================== Konsol Versiyonu (Raylib Gerektirmez) ====================
set(CONSOLE_SOURCES
    main_backup.c
    src/route_solver.c
    src/min_heap.c
    src/bst_tree.c
    src/graph_loader.c
)

add_executable(MetroConsole ${CONSOLE_SOURCES})

target_include_directories(MetroConsole PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Windows için ek kütüphaneler (konsol için sadece temel olanlar)
if(WIN32)
    target_link_libraries(MetroConsole PRIVATE)
endif()