#ifndef STRUCTS_H
#define STRUCTS_H

#include <limits.h> // INT_MAX için gerekli

// --- 1. Sabit Tanımlamalar ---
#define MAX_STOPS 300
#define MAX_NAME_LEN 30 
// INF'i long long'a uyumlu olarak tanımla
#define INF 2147483647L // INT_MAX (L soneki, long tipine işaret eder)




// --- 2. İLERİ BİLDİRİMLER (Forward Declarations) ---
// Edge ve Stop yapıları birbirine işaret ettiği için bu zorunludur.
typedef struct Edge Edge;
typedef struct Stop Stop;


// --- 3. TEMEL YAPILAR ---

// A. BST Düğümü Yapısı (Ağaç)
typedef struct Node {
    char name[MAX_NAME_LEN]; 
    int stop_id; 
    struct Node *left; 
    struct Node *right; 
} Node;

// B. Durak Yapısı (Stop) - Graph Düğümü
typedef struct Stop {
    int id;
    char name[MAX_NAME_LEN];
    char line[10];
    Edge* head; // Komşuluk listesinin başı (Edge'e işaret eder)
} Stop;

// C. Ana Graf Yapısı (Graph)
typedef struct Graph {
    Stop stops[MAX_STOPS]; // Tüm durakları tutan dizi
    int num_stops;         // Grafdaki toplam durak sayısı
} Graph;

// D. Kenar Yapısı (Edge) - Bağlı Liste Düğümü
typedef struct Edge {
    int target_id;
    int duration;
    char line[10];
    int congestion_score; // Canlı Yoğunluk Puanı (1-5)
    int is_closed;        // Kapalı yol bayrağı (1=Kapalı, 0=Açık)
    struct Edge* next;    // Bağlı listedeki bir sonraki kenara işaretçi
} Edge;

// E. Min-Heap Düğüm Yapısı (Dijkstra)
// includes/structs.h dosyasında:

typedef struct HeapNode {
    int stop_id; 
    // int distance; // ESKİ HALİ (Hata verir)
    long long distance; // YENİ HALİ (Doğru Tip)
} HeapNode;

// F. Min-Heap Yapısı (Öncelik Kuyruğu)
typedef struct MinHeap {
    HeapNode* array; 
    int size; 
    int capacity;
    int* pos; // Konum dizisi
} MinHeap;


#endif // STRUCTS_H