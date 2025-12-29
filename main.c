#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>

#include "includes/structs.h"
#include "includes/algorithms.h"
#include "includes/ui.h"

// Kuyruk için extern tanımlamalar
#define QUEUE_MAX_SIZE MAX_STOPS
#define MAX_NAME_LEN_STR "99"
extern int queue_arr[QUEUE_MAX_SIZE];
extern int front;
extern int rear;

int main() {
    const char* file_name = "metro.csv";

    Graph city_graph;
    Node* root_bst = NULL;
    
    // Rastgele sayı üreteci başlat (yoğunluk simülasyonu için)
    srand((unsigned int)time(NULL));

    // a. Grafı belleğe yükle
    parse_and_build_graph(file_name, &city_graph);
    printf("Graf basariyla olusturuldu. Toplam durak: %d\n", city_graph.num_stops);
    
    // b. Başlangıç yoğunluk değerlerini oluştur
    generate_random_congestion(&city_graph);
    printf("Canli yogunluk verileri olusturuldu.\n");

    // c. BST'yi doldur
    for (int i = 1; i <= city_graph.num_stops; i++) {
        if (city_graph.stops[i-1].id != -1) {
            root_bst = insert_bst(root_bst, city_graph.stops[i-1].name, city_graph.stops[i-1].id);
        }
    }
    printf("BST (Arama Agaci) basariyla olusturuldu.\n");

    //UI Durumunu Başlat
    UIState uiState = {0};
    uiState.selectedStartLine = -1;
    uiState.selectedEndLine = -1;
    uiState.selectedStartStopId = -1;
    uiState.selectedEndStopId = -1;
    uiState.selectedStartName[0] = '\0';
    uiState.selectedEndName[0] = '\0';
    uiState.showStartDropdown = false;
    uiState.showEndDropdown = false;
    uiState.startDropdownScroll = 0;
    uiState.endDropdownScroll = 0;
    uiState.showRouteWindow = false;
    uiState.routeLength = 0;
    uiState.totalTime = 0;
    uiState.isDijkstraRoute = false;



    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Anti-aliasing
    InitWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Istanbul Metro Rota Planlayici");
    SetTargetFPS(60);

    //Ana Döngü
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){20, 20, 40, 255});
        DrawMainUI(&city_graph, &uiState, root_bst);
        EndDrawing();
    }
    CloseWindow();

    free_bst(root_bst);
    printf("BST bellekten temizlendi.\n");

    free_graph_edges(&city_graph);
    printf("Graf kenarlari bellekten temizlendi.\n");
    
    return 0;
}