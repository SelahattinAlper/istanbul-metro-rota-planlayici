#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

// Proje başlık dosyaları (structs.h'te tüm yapılar, algorithms.h'te tüm prototipler olmalı)
// DİKKAT: include yollarının düzeltildiğini varsayıyoruz (örn: #include "../includes/structs.h")
#include "includes/structs.h"   
#include "includes/algorithms.h"  

// Global Kuyruk Değişkenleri (BFS için)
// Bu değişkenler route_solver.c içinde de tanımlı olduğu için, 
// burada sadece prototip veya extern olarak çağrılmalıdır. Basitlik için burada tekrar tanımlanmıştır.
#define QUEUE_MAX_SIZE MAX_STOPS 
extern int queue_arr[QUEUE_MAX_SIZE];
extern int front;
extern int rear;

// --- Ana Fonksiyon (MAIN) ---
int main() {
    
    // Proje ayarları
    const char* file_name = "metro.csv"; 
    
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

    // --- 2. Kullanıcı Girişleri ve Arama (Kişi A'nın Görevi) ---
    
    char start_name[MAX_NAME_LEN], end_name[MAX_NAME_LEN];
    printf("\n--- ROTA PLANLAYICIYA HOS GELDINIZ ---\n");
    
    // Örnek: Kadikoy ve Dudullu_M8 kullanabiliriz
    printf("Baslangic duragini girin (Ornek: Kadikoy): ");
    scanf("%s", start_name);
    
    printf("Hedef duragi girin (Ornek: Dudullu_M8): ");
    scanf("%s", end_name);
    
    // BST ile ID'leri bulma
    int start_id = search_bst(root_bst, start_name);
    int end_id = search_bst(root_bst, end_name);
    
    if (start_id == -1 || end_id == -1) {
        printf("\nHata: Baslangic veya hedef durak bulunamadi. Varsayilan (1 -> 109) kullaniliyor.\n");
        start_id = 1; // Kadikoy
        end_id = 109; // Dudullu_M8
        printf("Secilen Rota: Kadikoy (1) -> Dudullu_M8 (109)\n");
    } else {
        printf("Secilen Rota: %s (%d) -> %s (%d)\n", start_name, start_id, end_name, end_id);
    }
    
    // --- 3. Rota Hesaplama ve Sonuç (Kişi A'nın Görevi) ---

    // a. En Kısa Süre (Dijkstra - src/route_solver.c ve src/min_heap.c)
    printf("\n--- EN KISA SURE ROTASI (DIJKSTRA) ---\n");
    dijkstra_shortest_time(&city_graph, start_id, end_id);

    // b. En Az Aktarma/Durak (BFS - src/route_solver.c)
    printf("\n--- EN AZ AKTARMA ROTASI (BFS) ---\n");
    bfs_min_transfers(&city_graph, start_id, end_id);

    // 4. Bellek Temizliği (Gerçek projede tüm malloc'lar temizlenmelidir)
    // free_bst(root_bst);
    // free_graph_edges(&city_graph);
    
    return 0;
}