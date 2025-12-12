#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// Bu include'lar, structs.h ve algorithms.h dosyalarındaki tanımları içerir
#include "../includes/structs.h"
#include "../includes/algorithms.h"


#define MAX_STOPS 300
extern int queue_arr[MAX_STOPS];
extern int front;
extern int rear;

// --- Global Değişkenler (Kuyruk için) ---
// ... (Kuyruk tanımı aynı kalır)
#define QUEUE_MAX_SIZE MAX_STOPS
int queue_arr[QUEUE_MAX_SIZE];
int front = -1, rear = -1;

// --- Kuyruk Fonksiyonları ---
// ... (enqueue, dequeue, is_queue_empty fonksiyonları aynı kalır)

// Yolu recursive olarak yazdıran yardımcı fonksiyon
void print_path(Graph* graph, int end_id, const int parent[]) {
    if (parent[end_id] == -1) {
        printf("%s (%d)", graph->stops[end_id - 1].name, end_id);
        return;
    }
    print_path(graph, parent[end_id], parent);
    printf(" -> %s (%d)", graph->stops[end_id - 1].name, end_id);
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


// Dijkstra Algoritması (GÜNCELLENMİŞ)

// src/route_solver.c dosyası (Dijkstra)

void dijkstra_shortest_time(Graph* graph, int start_id, int end_id) {
    // dist dizisi long long olmalı
    long long dist[MAX_STOPS + 1]; 
    int parent[MAX_STOPS + 1]; 
    
    // INF değeri de long long olmalı (structs.h'te LONG_INF tanımlanmadığı için INT_MAX kullanmaya devam ediyoruz)
    
    for (int i = 1; i <= graph->num_stops; i++) {
        dist[i] = INF; 
        parent[i] = -1;
    }
    
    dist[start_id] = 0; // Başlangıç mesafesini sıfırla

    MinHeap* minHeap = createMinHeap(graph->num_stops);
    
    
    // KRİTİK BAŞLATMA: MinHeap'in Doldurulması
    // Heap'i tüm düğümlerle, başlangıç mesafeleriyle ve pozisyonlarla doldur.
    minHeap->size = 0;
    for (int i = 1; i <= graph->num_stops; i++) {
        minHeap->array[minHeap->size].stop_id = i;
        
        // dist dizisinden doğru long long mesafeyi al
        minHeap->array[minHeap->size].distance = dist[i]; 
        
        minHeap->pos[i] = minHeap->size;
        minHeap->size++;
    }
    
    // Burası, MinHeap'teki başlangıç düğümünün mesafesinin 0'a güncellenmesini
    // ve köke taşınmasını garanti eder.
    decrease_key(minHeap, start_id, 0);

    while (minHeap->size > 0) {
        HeapNode extracted = extract_min(minHeap);
        int u_id = extracted.stop_id;
        long long u_dist = extracted.distance; // long long olarak okundu

        if (u_dist == INF) break; 
        if (u_id == end_id) break; 

        Stop* u_stop = &graph->stops[u_id - 1];
        Edge* edge = u_stop->head;

        // src/route_solver.c dosyasında, dijkstra_shortest_time fonksiyonu içinde:

// ... (while (edge != NULL) döngüsünün içinde)

        while (edge != NULL) {
            int v_id = edge->target_id;
            long long current_cost = calculate_dynamic_cost(edge); // long long maliyet

            if (current_cost >= INF) { // Kapalı yol veya taşma
                edge = edge->next;
                continue;
            }

            // Gevşetme (Relaxation) işlemi
            // KRİTİK KONTROL: dist[u_id] != INF kontrolü, dist[u_id] + current_cost'un taşmasını önler.
            // Ayrıca, Heap'e eklenecek yeni mesafenin (dist[u_id] + current_cost) eski mesafeden (dist[v_id]) KÜÇÜK olması gerekir.
            if (dist[u_id] != INF && dist[u_id] + current_cost < dist[v_id]) {
                
                dist[v_id] = dist[u_id] + current_cost;
                parent[v_id] = u_id;

                // Heap'e ekle veya güncelle
                // Eğer Heap'te varsa, decrease_key ile güncelle (artık long long tipini kabul ediyor)
                if (is_in_min_heap(minHeap, v_id)) {
                    decrease_key(minHeap, v_id, dist[v_id]);
                }
                // Eğer Heap'te yoksa, buraya ekleme mantığı gelmeli. 
                // (Mevcut yapınızda, MinHeap ilk başta tüm düğümleri içerdiği için bu adım genellikle gereksizdir.)
            }
            edge = edge->next;
        }
// ...
    }
    
    // 4. Sonuçları Yazdırma
    if (dist[end_id] != INF) { 
        printf("Toplam Sure: %lld dakika\n", dist[end_id]); 
        printf("Rota: ");
        print_path(graph, end_id, parent);
        printf("\n");
    } else {
        printf("Hedefe ulasilamadi.\n");
    }
    // ... (Bellek temizliği)
}


//QUEUE
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

void bfs_min_transfers(Graph* graph, int start_id, int end_id) {
    int level[MAX_STOPS + 1]; // Başlangıçtan bu durağa minimum durak sayısını tutar
    int parent[MAX_STOPS + 1]; 
    
    // Başlatma (level dizisini INF olarak ayarla)
    for (int i = 1; i <= graph->num_stops; i++) {
        level[i] = INF; 
        parent[i] = -1;
    }
    
    // Kuyruk sıfırlama (Her çağrıda temizlenmeli)
    front = -1; rear = -1; 
    
    level[start_id] = 0;
    enqueue(start_id);
    
    // ===================================
    // KRİTİK EKSİK KISIM: BFS DÖNGÜSÜ
    // ===================================
    while (!is_queue_empty()) {
        int u_id = dequeue();

        // Hedefe ulaşıldıysa döngüyü kır
        if (u_id == end_id) break;

        Stop* u_stop = &graph->stops[u_id - 1];
        Edge* edge = u_stop->head;

        while (edge != NULL) {
            int v_id = edge->target_id;

            // Eğer komşu durak daha önce ziyaret edilmediyse (level == INF)
            if (level[v_id] == INF) {
                // Seviyeyi güncelle (1 adım daha uzak)
                level[v_id] = level[u_id] + 1; 
                // Parent'ı ata
                parent[v_id] = u_id;
                // Kuyruğa ekle
                enqueue(v_id);
            }
            edge = edge->next;
        }
    } // 
    
    // KRİTİK: BFS Sonuç Yazdırma
    if (level[end_id] != INF) {
        printf("Minimum Durak Sayisi: %d\n", level[end_id]);
        printf("Rota: ");
        print_path(graph, end_id, parent);
        printf("\n");
    } else {
        printf("Hedefe ulasilamadi.\n");
    }
}