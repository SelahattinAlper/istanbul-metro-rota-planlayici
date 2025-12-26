#ifndef UI_H
#define UI_H

#include <raylib.h>
#include "structs.h"

// --- Pencere boyutları ---
#define MAIN_WINDOW_WIDTH 1400
#define MAIN_WINDOW_HEIGHT 800
#define ROUTE_WINDOW_WIDTH 1100
#define ROUTE_WINDOW_HEIGHT 600

// --- UI Sabitleri ---
#define MAX_VISIBLE_STOPS 6
#define STOPS_PER_ROW 4
#define CIRCLE_RADIUS 22
#define CIRCLE_SPACING 55

// --- Metro Hat İndeksleri ---
#define LINE_M1A 0
#define LINE_M1B 1
#define LINE_M2 2
#define LINE_M3 3
#define LINE_M4 4
#define LINE_M5 5
#define LINE_M6 6
#define LINE_M7 7
#define LINE_M8 8
#define LINE_M9 9
#define LINE_M11 10
#define LINE_MARMARAY 11
#define LINE_COUNT 12

// --- UI Durumu Yapısı ---
typedef struct {
    int selectedStartLine;      // Seçilen başlangıç hattı indeksi
    int selectedEndLine;        // Seçilen bitiş hattı indeksi
    int selectedStartStopId;    // Seçilen başlangıç durağı ID
    int selectedEndStopId;      // Seçilen bitiş durağı ID
    char selectedStartName[MAX_NAME_LEN];
    char selectedEndName[MAX_NAME_LEN];
    
    bool showStartDropdown;     // Başlangıç dropdown açık mı?
    bool showEndDropdown;       // Bitiş dropdown açık mı?
    int startDropdownScroll;    // Başlangıç dropdown scroll pozisyonu
    int endDropdownScroll;      // Bitiş dropdown scroll pozisyonu
    
    bool showRouteWindow;       // Rota penceresi açık mı?
    int routePath[MAX_STOPS];   // Hesaplanan rota (durak ID'leri)
    int routeLength;            // Rota uzunluğu
    int totalTime;              // Toplam süre (dakika)
    bool isDijkstraRoute;       // Dijkstra mı BFS mi?
} UIState;

// --- Hat Bilgisi Yapısı ---
typedef struct {
    const char* name;
    const char* displayName;
    Color color;
} LineInfo;

// --- Global hat bilgileri ---
extern LineInfo lineInfos[LINE_COUNT];

// --- Fonksiyon Prototipleri ---

// Renk fonksiyonları
Color GetLineColor(const char* lineName);
int GetLineIndex(const char* lineName);

// Hat dairelerini çizme
void DrawLineCircles(int x, int y, int* selectedLine, bool isHovered[LINE_COUNT]);

// Dropdown liste çizme
void DrawStopDropdown(int x, int y, int width, Graph* graph, int selectedLine, 
                      int* selectedStopId, char* selectedName, bool* showDropdown, 
                      int* scrollPos);

// Rota penceresi çizme
void DrawRouteWindow(Graph* graph, UIState* state);

// Ok çizme (duraklar arası)
void DrawArrow(int x1, int y1, int x2, int y2, Color color);

// Mouse kontrolü
bool IsMouseOverCircle(int cx, int cy, int radius);
bool IsMouseOverRect(int x, int y, int width, int height);

// Hatta göre durakları filtrele
int GetStopsForLine(Graph* graph, int lineIndex, int* stopIds, char stopNames[][MAX_NAME_LEN]);

// Ana UI çizimi
void DrawMainUI(Graph* graph, UIState* state, Node* bst);

// Rota hesaplama (UI için modifiye edilmiş)
void CalculateRouteForUI(Graph* graph, UIState* state, bool useDijkstra);

#endif // UI_H
