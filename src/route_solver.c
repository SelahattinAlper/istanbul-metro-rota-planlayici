#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../includes/structs.h"
#include "../includes/algorithms.h"

extern int queue_arr[MAX_STOPS];
extern int front;
extern int rear;


#define QUEUE_MAX_SIZE MAX_STOPS
int queue_arr[QUEUE_MAX_SIZE];
int front = -1, rear = -1;

// --- Kuyruk Fonksiyonları ---

// Yolu recursive olarak yazdıran yardımcı fonksiyon
void print_path(Graph* graph, int end_id, const int parent[]) {
    if (parent[end_id] == -1) {
        printf("%s ", graph->stops[end_id - 1].name);
        return;
    }
    print_path(graph, parent[end_id], parent);
    printf("-> %s ", graph->stops[end_id - 1].name);
}

// *** ÇOK KRİTERLİ AĞIRLIK HESAPLAMA YARDIMCISI ***
// Bu fonksiyon, Kapalı Yol ve Yoğunluk kriterlerini ağırlığa yansıtır.
long long calculate_dynamic_cost(Edge* edge) {
    // 1. Kapalı Yol Kontrolü (Dinamik Engel)
    if (edge->is_closed) {
        return INF; // Kapalı yolu sonsuz maliyetle cezalandır.
    }

    // 2. Yoğunluk Maliyeti Hesaplama
    // Yoğunluk çarpanı: 1 (Düşük) - 5 (Yüksek). Her puan %20 ekstra süre cezası ekler.
    // Örneğin, yoğunluk 3 ise çarpan 1 + (2 * 0.2) = 1.4 olur.
    // 1 puan = 0, 2 puan = %20, 5 puan = %80 ceza
    double congestion_factor = (double)(edge->congestion_score - 1) * 0.2;

    // Total maliyet = Temel Süre * (1 + Çarpan)
    long long current_cost = (long long)(edge->duration * (1.0 + congestion_factor));

    // NOT: Aktarma cezası (aktarma sayısı) eklemek için bu fonksiyona
    // önceki düğümün hattı ve toplam aktarma sayısı bilgisi de geçirilmelidir.

    return current_cost;
}

long long dijkstra_shortest_time(Graph* graph, int start_id, int end_id, int* out_parent) {
    // dist dizisi long long olmalı
    long long dist[MAX_STOPS + 1]; 
    int parent[MAX_STOPS + 1]; 
    
    for (int i = 1; i <= graph->num_stops; i++) {
        dist[i] = INF; 
        parent[i] = -1;
    }
    
    dist[start_id] = 0; // Başlangıç mesafesini sıfırla

    MinHeap* minHeap = createMinHeap(graph->num_stops);
    
    // KRİTİK BAŞLATMA: MinHeap'in Doldurulması
    minHeap->size = 0;
    for (int i = 1; i <= graph->num_stops; i++) {
        minHeap->array[minHeap->size].stop_id = i;
        minHeap->array[minHeap->size].distance = dist[i]; 
        minHeap->pos[i] = minHeap->size;
        minHeap->size++;
    }
    
    decrease_key(minHeap, start_id, 0);

    while (minHeap->size > 0) {
        HeapNode extracted = extract_min(minHeap);
        int u_id = extracted.stop_id;
        long long u_dist = extracted.distance;

        if (u_dist == INF) break; 
        if (u_id == end_id) break; 

        Stop* u_stop = &graph->stops[u_id - 1];
        Edge* edge = u_stop->head;

        while (edge != NULL) {
            int v_id = edge->target_id;
            long long current_cost = calculate_dynamic_cost(edge);

            if (current_cost >= INF) {
                edge = edge->next;
                continue;
            }

            if (dist[u_id] != INF && dist[u_id] + current_cost < dist[v_id]) {
                dist[v_id] = dist[u_id] + current_cost;
                parent[v_id] = u_id;

                if (is_in_min_heap(minHeap, v_id)) {
                    decrease_key(minHeap, v_id, dist[v_id]);
                }
            }
            edge = edge->next;
        }
    }

    // Heap temizliği
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
    
    // Parent dizisini dışarı kopyala (eğer isteniyorsa)
    if (out_parent != NULL) {
        for (int i = 1; i <= graph->num_stops; i++) {
            out_parent[i] = parent[i];
        }
    } else {
        // Konsol çıktısı (eski davranış)
        if (dist[end_id] != INF) { 
            printf("Toplam Sure: %lld dakika\n", dist[end_id]); 
            printf("Rota: ");
            print_path(graph, end_id, parent);
            printf("\n");
        } else {
            printf("Hedefe ulasilamadi.\n");
        }
    }
    
    return dist[end_id];
}



int is_queue_empty() {
    return front == -1;
}

void enqueue(int stop_id) {
    if (rear == MAX_STOPS - 1) {
        // Kuyruk dolu
        fprintf(stderr, "Hata: Kuyruk dolu!\n");
        return; 
    }
    if (front == -1) { // Kuyruk ilk kez doluyorsa
        front = 0;
    }
    rear++;
    queue_arr[rear] = stop_id;
}

int dequeue() {
    if (is_queue_empty()) {
        // Hata: Kuyruk boş
        return -1; 
    }
    int stop_id = queue_arr[front];
    if (front == rear) { // Kuyruktaki son eleman
        front = -1;
        rear = -1;
    } else {
        front++;
    }
    return stop_id;
}
// BFS Algoritması (Aynı kalır, sadece minimum durak sayısını hesaplar)
// src/route_solver.c dosyasında, bfs_min_transfers fonksiyonunun TAMAMI:

int bfs_min_transfers(Graph* graph, int start_id, int end_id, int* out_parent) {
    int level[MAX_STOPS + 1];
    int parent[MAX_STOPS + 1]; 
    
    for (int i = 1; i <= graph->num_stops; i++) {
        level[i] = INF; 
        parent[i] = -1;
    }
    
    // Kuyruk sıfırlama
    front = -1; rear = -1; 
    
    level[start_id] = 0;
    enqueue(start_id);
    
    while (!is_queue_empty()) {
        int u_id = dequeue();

        if (u_id == end_id) break;

        Stop* u_stop = &graph->stops[u_id - 1];
        Edge* edge = u_stop->head;

        while (edge != NULL) {
            int v_id = edge->target_id;

            if (level[v_id] == INF) {
                level[v_id] = level[u_id] + 1; 
                parent[v_id] = u_id;
                enqueue(v_id);
            }
            edge = edge->next;
        }
    }
    
    // Parent dizisini dışarı kopyala (eğer isteniyorsa)
    if (out_parent != NULL) {
        for (int i = 1; i <= graph->num_stops; i++) {
            out_parent[i] = parent[i];
        }
    } else {
        // Konsol çıktısı (eski davranış)
        if (level[end_id] != INF) {
            printf("Minimum Durak Sayisi: %d\n", level[end_id]);
            printf("Rota: ");
            print_path(graph, end_id, parent);
            printf("\n");
        } else {
            printf("Hedefe ulasilamadi.\n");
        }
    }
    
    return level[end_id];
}

