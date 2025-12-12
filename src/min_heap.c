#include <stdlib.h>
#include <limits.h>
#include "../includes/structs.h" 
#include "../includes/algorithms.h"

// NOT: INF'in structs.h'te long long'a uyumlu bir değer (örn. INT_MAX) olduğunu varsayıyoruz.

// 1. Min-Heap Yapısını Oluşturma
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    if (minHeap == NULL) return NULL; 

    // HeapNode'un long long içerdiğini varsayarak bellek ayır
    minHeap->pos = (int*)malloc((capacity + 1) * sizeof(int)); 
    minHeap->array = (HeapNode*)malloc((capacity + 1) * sizeof(HeapNode));
    
    if (minHeap->pos == NULL || minHeap->array == NULL) {
        free(minHeap->pos);
        free(minHeap->array);
        free(minHeap);
        return NULL;
    }
    
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

// 2. Takas İşlevi
void swap_heap_nodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// 3. Min-Heap Özelliğini Koruma (Heapify)
void min_heapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    // Karşılaştırmalar long long tipindeki 'distance' alanını kullanır
    if (left < minHeap->size && minHeap->array[left].distance < minHeap->array[smallest].distance)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right].distance < minHeap->array[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        // Pos dizisini güncelle ve takas yap
        minHeap->pos[minHeap->array[smallest].stop_id] = idx;
        minHeap->pos[minHeap->array[idx].stop_id] = smallest;

        swap_heap_nodes(&minHeap->array[smallest], &minHeap->array[idx]);
        min_heapify(minHeap, smallest);
    }
}

// 4. Minimum Elemanı Çıkarma (extract_min)
HeapNode extract_min(MinHeap* minHeap) {
    if (minHeap->size == 0) {
        // HeapNode.distance long long olduğu için INF'i long long olarak döndür
        HeapNode empty = {-1, (long long)INF}; 
        return empty;
    }

    HeapNode root = minHeap->array[0];
    
    HeapNode lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root.stop_id] = minHeap->size - 1; 
    minHeap->pos[lastNode.stop_id] = 0;             

    minHeap->size--;
    min_heapify(minHeap, 0);

    return root;
}

// 5. Anahtar Değerini Azaltma (decrease_key) - Dijkstra için KRİTİK
// new_distance artık long long tipindedir
void decrease_key(MinHeap* minHeap, int stop_id, long long new_distance) { 
    int i = minHeap->pos[stop_id]; 
    
    // new_distance, mevcut distance'tan büyükse veya index geçersizse geri dön
    if (i >= minHeap->size || new_distance > minHeap->array[i].distance) return;

    minHeap->array[i].distance = new_distance;

    // Heap-up işlemi: Düğümü yukarı taşı (long long karşılaştırması kullanır)
    while (i > 0 && minHeap->array[i].distance < minHeap->array[(i - 1) / 2].distance) {
        // Pos dizisini güncelle
        minHeap->pos[minHeap->array[i].stop_id] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2].stop_id] = i;
        
        swap_heap_nodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// 6. Heap'te Var mı Kontrolü
int is_in_min_heap(MinHeap* minHeap, int stop_id) {
    return minHeap->pos[stop_id] < minHeap->size;
}