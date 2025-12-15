#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "includes/structs.h"
#include "includes/algorithms.h"

// Global Kuyruk Değişkenleri (BFS için)
// Bu değişkenler route_solver.c içinde de tanımlı olduğu için,
// burada sadece prototip veya extern olarak çağrılmalıdır. Basitlik için burada tekrar tanımlanmıştır.
#define QUEUE_MAX_SIZE MAX_STOPS
#define MAX_NAME_LEN_STR "99"
extern int queue_arr[QUEUE_MAX_SIZE];
extern int front;
extern int rear;


int main() {

    const char* file_name = "../istanbul-metro-rota-planlayici/metro.csv";

    // 1. Yapıları Başlatma
    Graph city_graph;
    // BST kökü başlatılır
    Node* root_bst = NULL;

    // a. Grafı belleğe yükle (src/graph_loader.c)
    // Bu, graph_loader.c'deki parse_and_build_graph fonksiyonunu çağırır.
    parse_and_build_graph(file_name, &city_graph);
    printf("Graf basariyla olusturuldu. Toplam durak: %d\n", city_graph.num_stops);

    // b. BST'yi doldur (src/bst_tree.c)
    for (int i = 1; i <= city_graph.num_stops; i++) {
        if (city_graph.stops[i-1].id != -1) {
            root_bst = insert_bst(root_bst, city_graph.stops[i-1].name, city_graph.stops[i-1].id);
        }
    }
    printf("BST (Arama Agaci) basariyla olusturuldu.\n");


    int start_id = -1,end_id = -1;
    char start_name[MAX_NAME_LEN], end_name[MAX_NAME_LEN];
    int choice;
    int valid_stops = 0;//Duraklarin Gecerli Olup Olmadigini tutar.


    START_OVER:
    valid_stops = 0;

    while (!valid_stops) {
        printf("\n--- ROTA PLANLAYICIYA HOS GELDINIZ ---\n");
        printf("Baslangic duragini girin (Ornek: Kadikoy): ");
        scanf("%" MAX_NAME_LEN_STR "s", start_name);
        printf("Hedef duragi girin (Ornek: Dudullu_M8): ");
        scanf("%" MAX_NAME_LEN_STR "s", end_name);
        // BST ile ID'leri bulma
        start_id = search_bst(root_bst, start_name);
        end_id = search_bst(root_bst, end_name);
        if (start_id == -1 || end_id == -1) {
            printf("\nHata: Baslangic veya hedef durak bulunamadi. Varsayilan (1 -> 109) kullaniliyor.\n");
            start_id = 1; // Kadikoy
            end_id = 109; // Dudullu_M8
            printf("Secilen Rota: Kadikoy (1) -> Dudullu_M8 (109)\n");
            valid_stops= 1;
        } else {
            printf("Secilen Rota: %s (%d) -> %s (%d)\n", start_name, start_id, end_name, end_id);
            valid_stops = 1;
        }
    }
    while (1) {
        printf("\nLutfen bir islem seciniz:\n");
        printf("1-En Kisa Sure Hesaplama (Dijkstra)\n");
        printf("2-En Az Durak Hesaplama (BFS)\n");
        printf("3-Yeni Rota Belirle\n");
        printf("4-Cikis\n");
        printf("Seciminiz: ");
        if (scanf("%d", &choice) != 1) {
            printf("\nHata: Gecersiz giris. Lutfen bir sayi girin.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        switch (choice) {
            case 1:
                // a. En Kısa Süre (Dijkstra - src/route_solver.c ve src/min_heap.c)
                printf("\n--- EN KISA SURE ROTASI (DIJKSTRA) ---\n");
                dijkstra_shortest_time(&city_graph, start_id, end_id);
                break;
            case 2:
                // b. En Az Aktarma/Durak (BFS - src/route_solver.c)
                printf("\n--- EN AZ AKTARMA ROTASI (BFS) ---\n");
                bfs_min_transfers(&city_graph, start_id, end_id);
                break;
            case 3:
                printf("\n--- YENI ROTA BELIRLEME ---\n");
                goto START_OVER;
            case 4:
                exit(0);
            default:
                printf("\nHata: Gecersiz secim (%d). Lutfen 1, 2, 3 veya 4 girin.\n", choice);
                break;
        }
    }

    return 0;
}