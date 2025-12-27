#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "algorithms.h"

// 1. Yardımcı Fonksiyon: Kenar Ekleme
void add_edge(Stop* stop, int target_id, int duration, const char* line_name, int congestion_score, int is_closed) {
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    if (new_edge == NULL) { 
        perror("Bellek ayirma hatasi (Edge)"); 
        exit(EXIT_FAILURE); 
    }
    
    new_edge->target_id = target_id;
    new_edge->duration = duration;
    strncpy(new_edge->line, line_name, 9);
    new_edge->line[9] = '\0';
    new_edge->congestion_score = congestion_score;

    // Bağlı listenin başına ekle
    new_edge->next = stop->head;
    stop->head = new_edge;
}

// 2. Ana Fonksiyon: CSV Dosyasını Ayrıştırma ve Grafı Oluşturma
void parse_and_build_graph(const char* filename, Graph* graph) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("HATA: Metro veri dosyasi (metro.csv) acilamadi");
        exit(EXIT_FAILURE);
    }
    
    graph->num_stops = 0;
    char line[512]; 
    
    // Tüm durakları initialize et
    for (int i = 0; i < MAX_STOPS; i++) {
        graph->stops[i].id = -1;
        graph->stops[i].head = NULL;
    }

    //CSV Başlık Satırını Atla
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return;
    }

    // 1. GEÇİŞ: Temel Durak Bilgilerini ve İleri Kenarları (u -> v) Oluşturma
    rewind(file); 
    
    // Başlığı tekrar atla 
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char* tmp = strdup(line);
        if (tmp == NULL) { perror("Bellek hatasi"); exit(EXIT_FAILURE); }
        
        char* id_str = strtok(tmp, ",");
        char* name_str = strtok(NULL, ",");
        char* line_str = strtok(NULL, ",");
        char* neighbors_str = strtok(NULL, ",");
        
        if (id_str == NULL || name_str == NULL || line_str == NULL || neighbors_str == NULL) {
            free(tmp);
            continue;
        }

        int id = atoi(id_str);
        int index = id - 1;

        if (index < 0 || index >= MAX_STOPS) { free(tmp); continue; }

        // Ana Durak Yapısını Doldur
        graph->stops[index].id = id;
        strncpy(graph->stops[index].name, name_str, MAX_NAME_LEN - 1);
        strncpy(graph->stops[index].line, line_str, 9);
        if (id > graph->num_stops) graph->num_stops = id;

        // Komşulukları işle
        char* neighbors_copy = strdup(neighbors_str);
        char* neighbor_token = strtok(neighbors_copy, ";\n");
        
        while (neighbor_token != NULL) {
            int target_id, duration;
            if (sscanf(neighbor_token, "%d-%d", &target_id, &duration) == 2) {
                // İLERİ KENARI EKLE (u -> v)
                add_edge(&graph->stops[index], target_id, duration, line_str, 1, 0); 
            }
            neighbor_token = strtok(NULL, ";\n");
        }
        
        free(neighbors_copy);
        free(tmp);
    }

    // 2. GEÇİŞ: Geri dönüş kenarlarını (v -> u) ekleyerek grafı çift yönlü yapma
    
    rewind(file);
    
    // Başlığı tekrar atla 
    char header_buffer[512];
    if (fgets(header_buffer, sizeof(header_buffer), file) == NULL) { 
        fclose(file);
        return;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // 1. Yeni ve TEMİZ bir kopya al
        char* tmp = strdup(line); 
        if (tmp == NULL) { perror("Bellek hatasi"); exit(EXIT_FAILURE); }
        
        // 2. CSV alanlarını ayrıştır
        char* id_str = strtok(tmp, ",");
        strtok(NULL, ","); // Name atla
        char* line_str = strtok(NULL, ",");
        char* neighbors_str = strtok(NULL, ",");
        
        if (id_str == NULL || line_str == NULL || neighbors_str == NULL) {
            free(tmp);
            continue;
        }

        int u_id = atoi(id_str); // Başlangıç ID'si (u)
        
        // 3. Komşuluk dizesini (neighbors_str) işlemek için YENİ bir kopya al
        char* neighbors_copy = strdup(neighbors_str);
        char* neighbor_token = strtok(neighbors_copy, ";\n");

        while (neighbor_token != NULL) {
            int v_id, duration; // Hedef ID (v) ve süre
            
            if (sscanf(neighbor_token, "%d-%d", &v_id, &duration) == 2) {
                int v_index = v_id - 1; 

                // GERİ KENARI EKLE (v -> u)
                if (v_index >= 0 && v_index < MAX_STOPS && graph->stops[v_index].id != -1) { 
                    
                    // add_edge, v_id'yi kaynak olarak kullanır
                    add_edge(&graph->stops[v_index], u_id, duration, line_str, 1, 0);
                }
            }
            neighbor_token = strtok(NULL, ";\n");
        }
        free(neighbors_copy);
        free(tmp);
    }
    fclose(file);
}

// Rastgele yoğunluk değerleri ata
void generate_random_congestion(Graph* graph) {
    for (int i = 0; i < graph->num_stops; i++) {
        if (graph->stops[i].id == -1) continue;
        
        Edge* edge = graph->stops[i].head;
        while (edge != NULL) {
            // 1-5 arası rastgele yoğunluk değeri
            int random_val = rand() % 100;
            
            if (random_val < 50) {
                edge->congestion_score = 1 + (rand() % 2);  // 1-2: Sakin
            } else if (random_val < 80) {
                edge->congestion_score = 3;  // Orta
            } else if (random_val < 95) {
                edge->congestion_score = 4;  // Yoğun
            } else {
                edge->congestion_score = 5;  // Çok yoğun
            }
            
            edge = edge->next;
        }
    }
}