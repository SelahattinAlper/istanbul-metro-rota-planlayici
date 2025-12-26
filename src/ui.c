#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include "../includes/structs.h"
#include "../includes/algorithms.h"
#include "../includes/ui.h"

// --- Global Hat Bilgileri ---
LineInfo lineInfos[LINE_COUNT] = {
    {"M1A", "M1A", {227, 6, 19, 255}},       // Kırmızı
    {"M1B", "M1B", {227, 6, 19, 255}},       // Kırmızı
    {"M2", "M2", {0, 166, 82, 255}},         // Yeşil
    {"M3", "M3", {0, 114, 206, 255}},        // Mavi
    {"M4", "M4", {233, 30, 140, 255}},       // Pembe
    {"M5", "M5", {142, 68, 173, 255}},       // Mor
    {"M6", "M6", {243, 156, 18, 255}},       // Turuncu
    {"M7", "M7", {52, 152, 219, 255}},       // Açık Mavi
    {"M8", "M8", {149, 165, 166, 255}},      // Gri
    {"M9", "M9", {241, 196, 15, 255}},       // Sarı
    {"M11", "M11", {44, 62, 80, 255}},       // Lacivert
    {"Marmaray", "Marmaray", {26, 188, 156, 255}}  // Turkuaz
};

Color GetLineColor(const char* lineName) {
    for (int i = 0; i < LINE_COUNT; i++) {
        if (strcmp(lineName, lineInfos[i].name) == 0) {
            return lineInfos[i].color;
        }
    }
    return GRAY;
}

int GetLineIndex(const char* lineName) {
    for (int i = 0; i < LINE_COUNT; i++) {
        if (strcmp(lineName, lineInfos[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

bool IsMouseOverCircle(int cx, int cy, int radius) {
    Vector2 mouse = GetMousePosition();
    float dx = mouse.x - cx;
    float dy = mouse.y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

bool IsMouseOverRect(int x, int y, int width, int height) {
    Vector2 mouse = GetMousePosition();
    return (mouse.x >= x && mouse.x <= x + width && 
            mouse.y >= y && mouse.y <= y + height);
}

// --- Hatta Göre Durakları Getir ---
int GetStopsForLine(Graph* graph, int lineIndex, int* stopIds, char stopNames[][MAX_NAME_LEN]) {
    if (lineIndex < 0 || lineIndex >= LINE_COUNT) return 0;
    
    const char* targetLine = lineInfos[lineIndex].name;
    int count = 0;
    
    for (int i = 0; i < graph->num_stops && count < MAX_STOPS; i++) {
        if (graph->stops[i].id != -1 && strcmp(graph->stops[i].line, targetLine) == 0) {
            stopIds[count] = graph->stops[i].id;
            strncpy(stopNames[count], graph->stops[i].name, MAX_NAME_LEN - 1);
            stopNames[count][MAX_NAME_LEN - 1] = '\0';
            count++;
        }
    }
    return count;
}

// --- Hat Dairelerini Çiz ---
void DrawLineCircles(int x, int y, int* selectedLine, bool isHovered[LINE_COUNT]) {
    int currentX = x;
    
    // İlk satır: M1A, M1B, M2, M3, M4, M5
    for (int i = 0; i < 6 && i < LINE_COUNT; i++) {
        Color circleColor = lineInfos[i].color;
        
        // Seçili veya hover durumunda çerçeve çiz
        if (*selectedLine == i) {
            DrawCircle(currentX, y, CIRCLE_RADIUS + 4, WHITE);
        } else if (isHovered[i]) {
            DrawCircle(currentX, y, CIRCLE_RADIUS + 2, LIGHTGRAY);
        }
        
        DrawCircle(currentX, y, CIRCLE_RADIUS, circleColor);
        
        // Hat ismini ortala
        const char* name = lineInfos[i].displayName;
        int textWidth = MeasureText(name, 12);
        DrawText(name, currentX - textWidth/2, y - 6, 12, WHITE);
        
        // Tıklama kontrolü
        if (IsMouseOverCircle(currentX, y, CIRCLE_RADIUS)) {
            isHovered[i] = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedLine = i;
            }
        } else {
            isHovered[i] = false;
        }
        
        currentX += CIRCLE_SPACING;
    }
    
    // İkinci satır: M6, M7, M8, M9, M11, Marmaray
    currentX = x;
    int y2 = y + CIRCLE_SPACING;
    for (int i = 6; i < LINE_COUNT; i++) {
        Color circleColor = lineInfos[i].color;
        
        if (*selectedLine == i) {
            DrawCircle(currentX, y2, CIRCLE_RADIUS + 4, WHITE);
        } else if (isHovered[i]) {
            DrawCircle(currentX, y2, CIRCLE_RADIUS + 2, LIGHTGRAY);
        }
        
        DrawCircle(currentX, y2, CIRCLE_RADIUS, circleColor);
        
        const char* name = lineInfos[i].displayName;
        int textWidth = MeasureText(name, 10);
        DrawText(name, currentX - textWidth/2, y2 - 5, 10, WHITE);
        
        if (IsMouseOverCircle(currentX, y2, CIRCLE_RADIUS)) {
            isHovered[i] = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedLine = i;
            }
        } else {
            isHovered[i] = false;
        }
        
        currentX += CIRCLE_SPACING;
    }
}

// --- Dropdown Liste Çiz ---
void DrawStopDropdown(int x, int y, int width, Graph* graph, int selectedLine,
                      int* selectedStopId, char* selectedName, bool* showDropdown,
                      int* scrollPos) {
    
    // Ana dropdown kutusu
    Rectangle dropdownRect = {x, y, width, 35};
    Color boxColor = IsMouseOverRect(x, y, width, 35) ? (Color){60, 60, 80, 255} : (Color){40, 40, 60, 255};
    DrawRectangleRec(dropdownRect, boxColor);
    DrawRectangleLinesEx(dropdownRect, 2, (Color){100, 100, 130, 255});
    
    // Seçili durak ismini göster
    const char* displayText = (selectedName[0] != '\0') ? selectedName : "Durak Seciniz...";
    DrawText(displayText, x + 10, y + 10, 16, WHITE);
    
    // Ok işareti
    DrawTriangle(
        (Vector2){x + width - 20, y + 12},
        (Vector2){x + width - 10, y + 12},
        (Vector2){x + width - 15, y + 23},
        WHITE
    );
    
    // Tıklama ile aç/kapa
    if (IsMouseOverRect(x, y, width, 35) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *showDropdown = !(*showDropdown);
    }
    
    // Dropdown açıksa listeyi göster
    if (*showDropdown && selectedLine >= 0) {
        int stopIds[MAX_STOPS];
        char stopNames[MAX_STOPS][MAX_NAME_LEN];
        int stopCount = GetStopsForLine(graph, selectedLine, stopIds, stopNames);
        
        int listHeight = (stopCount > MAX_VISIBLE_STOPS ? MAX_VISIBLE_STOPS : stopCount) * 30;
        Rectangle listRect = {x, y + 38, width, listHeight};
        
        // Arkaplan
        DrawRectangleRec(listRect, (Color){30, 30, 50, 250});
        DrawRectangleLinesEx(listRect, 1, (Color){100, 100, 130, 255});
        
        // Scroll sınırları
        int maxScroll = stopCount - MAX_VISIBLE_STOPS;
        if (maxScroll < 0) maxScroll = 0;
        
        // Mouse wheel scroll
        if (IsMouseOverRect(x, y + 38, width, listHeight)) {
            int wheel = (int)GetMouseWheelMove();
            *scrollPos -= wheel;
            if (*scrollPos < 0) *scrollPos = 0;
            if (*scrollPos > maxScroll) *scrollPos = maxScroll;
        }
        
        // Durakları listele
        int visibleCount = (stopCount > MAX_VISIBLE_STOPS) ? MAX_VISIBLE_STOPS : stopCount;
        for (int i = 0; i < visibleCount && (i + *scrollPos) < stopCount; i++) {
            int idx = i + *scrollPos;
            int itemY = y + 38 + i * 30;
            Rectangle itemRect = {x + 2, itemY, width - 4, 28};
            
            bool isHovered = IsMouseOverRect(x + 2, itemY, width - 4, 28);
            bool isSelected = (stopIds[idx] == *selectedStopId);
            
            if (isSelected) {
                DrawRectangleRec(itemRect, (Color){80, 120, 180, 255});
            } else if (isHovered) {
                DrawRectangleRec(itemRect, (Color){60, 80, 120, 255});
            }
            
            DrawText(stopNames[idx], x + 10, itemY + 6, 14, WHITE);
            
            // Tıklama ile seç
            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *selectedStopId = stopIds[idx];
                strncpy(selectedName, stopNames[idx], MAX_NAME_LEN - 1);
                selectedName[MAX_NAME_LEN - 1] = '\0';
                *showDropdown = false;
            }
        }
        
        // Scroll bar (eğer gerekiyorsa)
        if (stopCount > MAX_VISIBLE_STOPS) {
            float scrollBarHeight = (float)listHeight * MAX_VISIBLE_STOPS / stopCount;
            float scrollBarY = y + 38 + (*scrollPos * (listHeight - scrollBarHeight) / maxScroll);
            DrawRectangle(x + width - 8, (int)scrollBarY, 6, (int)scrollBarHeight, (Color){100, 100, 130, 255});
        }
    }
}

// --- Ok Çizimi (Duraklar Arası) ---
void DrawArrow(int x1, int y1, int x2, int y2, Color color) {
    DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, 3, color);
    
    // Ok ucu
    float angle = atan2f(y2 - y1, x2 - x1);
    float arrowSize = 10;
    
    Vector2 p1 = {x2, y2};
    Vector2 p2 = {x2 - arrowSize * cosf(angle - 0.4f), y2 - arrowSize * sinf(angle - 0.4f)};
    Vector2 p3 = {x2 - arrowSize * cosf(angle + 0.4f), y2 - arrowSize * sinf(angle + 0.4f)};
    
    DrawTriangle(p1, p3, p2, color);
}

// --- Rota Penceresi Çizimi ---
void DrawRouteWindow(Graph* graph, UIState* state) {
    if (!state->showRouteWindow || state->routeLength == 0) return;
    
    // Pencere boyutları
    int winWidth = ROUTE_WINDOW_WIDTH;
    int winHeight = ROUTE_WINDOW_HEIGHT;
    int winX = (MAIN_WINDOW_WIDTH - winWidth) / 2;
    int winY = (MAIN_WINDOW_HEIGHT - winHeight) / 2;
    
    // Arkaplan (yarı saydam overlay)
    DrawRectangle(0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, (Color){0, 0, 0, 180});
    
    // Ana pencere
    DrawRectangle(winX, winY, winWidth, winHeight, (Color){25, 25, 45, 255});
    DrawRectangleLinesEx((Rectangle){winX, winY, winWidth, winHeight}, 3, (Color){100, 100, 150, 255});
    
    // Başlık
    const char* title = state->isDijkstraRoute ? "EN KISA SURE ROTASI (Dijkstra)" : "EN AZ DURAK ROTASI (BFS)";
    int titleWidth = MeasureText(title, 22);
    DrawText(title, winX + (winWidth - titleWidth) / 2, winY + 15, 22, WHITE);
    
    // Süre/Durak bilgisi
    char infoText[100];
    if (state->isDijkstraRoute) {
        sprintf(infoText, "Toplam Sure: %d dakika | Durak Sayisi: %d", state->totalTime, state->routeLength);
    } else {
        sprintf(infoText, "Toplam Durak Sayisi: %d", state->routeLength);
    }
    int infoWidth = MeasureText(infoText, 16);
    DrawText(infoText, winX + (winWidth - infoWidth) / 2, winY + 45, 16, (Color){200, 200, 200, 255});
    
    // Rota çizimi parametreleri
    int contentY = winY + 75;
    int stopBoxWidth = 100;
    int stopBoxHeight = 32;
    int arrowLength = 25;
    int rowSpacing = 60;
    int stopsPerRow = 6;
    
    int startX = winX + 30;
    int startY = contentY + 10;
    
    int currentX = startX;
    int currentY = startY;
    int stopsInRow = 0;
    
    for (int i = 0; i < state->routeLength; i++) {
        int stopId = state->routePath[i];
        Stop* stop = &graph->stops[stopId - 1];
        Color stopColor = GetLineColor(stop->line);
        
        // Durak kutusu
        Rectangle stopRect = {currentX, currentY, stopBoxWidth, stopBoxHeight};
        DrawRectangleRounded(stopRect, 0.3f, 8, stopColor);
        DrawRectangleRoundedLines(stopRect, 0.3f, 8, WHITE);
        
        // Durak ismi - kısaltılmış
        char shortName[16];
        strncpy(shortName, stop->name, 13);
        shortName[13] = '\0';
        if (strlen(stop->name) > 13) strcat(shortName, "..");
        
        int textWidth = MeasureText(shortName, 9);
        int textX = currentX + (stopBoxWidth - textWidth) / 2;
        DrawText(shortName, textX, currentY + 5, 9, WHITE);
        
        // Hat ismi
        int lineWidth = MeasureText(stop->line, 8);
        DrawText(stop->line, currentX + (stopBoxWidth - lineWidth) / 2, currentY + 18, 8, (Color){220, 220, 220, 255});
        
        stopsInRow++;
        
        // Sonraki durağa ok çiz
        if (i < state->routeLength - 1) {
            int nextStopId = state->routePath[i + 1];
            Stop* nextStop = &graph->stops[nextStopId - 1];
            Color arrowColor = WHITE;
            
            // Hat değişimi varsa farklı renk
            if (strcmp(stop->line, nextStop->line) != 0) {
                arrowColor = YELLOW;
            }
            
            // Satır sonuna geldiyse
            if (stopsInRow >= stopsPerRow) {
                // Aşağı ok
                int arrowStartX = currentX + stopBoxWidth / 2;
                int arrowStartY = currentY + stopBoxHeight;
                int arrowEndX = startX + stopBoxWidth / 2;
                int arrowEndY = currentY + rowSpacing;
                
                // Sağdan sola dönüş çizgisi
                DrawLineEx((Vector2){arrowStartX, arrowStartY}, (Vector2){arrowStartX, arrowStartY + 12}, 2, arrowColor);
                DrawLineEx((Vector2){arrowStartX, arrowStartY + 12}, (Vector2){arrowEndX, arrowStartY + 12}, 2, arrowColor);
                DrawArrow(arrowEndX, arrowStartY + 12, arrowEndX, arrowEndY, arrowColor);
                
                currentX = startX;
                currentY += rowSpacing;
                stopsInRow = 0;
            } else {
                // Sağa ok
                DrawArrow(currentX + stopBoxWidth, currentY + stopBoxHeight / 2,
                         currentX + stopBoxWidth + arrowLength - 5, currentY + stopBoxHeight / 2, arrowColor);
                currentX += stopBoxWidth + arrowLength;
            }
        }
    }
    
    // Kapatma butonu
    Rectangle closeBtn = {winX + winWidth - 45, winY + 10, 35, 25};
    bool closeHover = IsMouseOverRect(closeBtn.x, closeBtn.y, closeBtn.width, closeBtn.height);
    DrawRectangleRec(closeBtn, closeHover ? RED : (Color){180, 50, 50, 255});
    DrawText("X", closeBtn.x + 12, closeBtn.y + 5, 16, WHITE);
    
    if (closeHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state->showRouteWindow = false;
    }
}

// --- Rota Hesaplama (UI için) ---
// Bu fonksiyon merkezi Dijkstra/BFS fonksiyonlarını çağırır ve sonucu UIState'e kaydeder
void CalculateRouteForUI(Graph* graph, UIState* state, bool useDijkstra) {
    if (state->selectedStartStopId <= 0 || state->selectedEndStopId <= 0) return;
    
    int start_id = state->selectedStartStopId;
    int end_id = state->selectedEndStopId;
    
    // Parent dizisi
    int parent[MAX_STOPS + 1];
    
    if (useDijkstra) {
        // Merkezi Dijkstra fonksiyonunu çağır
        long long totalTime = dijkstra_shortest_time(graph, start_id, end_id, parent);
        state->totalTime = (int)totalTime;
        state->isDijkstraRoute = true;
    } else {
        // Merkezi BFS fonksiyonunu çağır
        bfs_min_transfers(graph, start_id, end_id, parent);
        state->isDijkstraRoute = false;
    }
    
    // Parent dizisinden rotayı oluştur
    if (parent[end_id] != -1 || start_id == end_id) {
        // Rotayı geriye doğru takip et
        int path[MAX_STOPS];
        int pathLen = 0;
        int current = end_id;
        
        while (current != -1 && pathLen < MAX_STOPS) {
            path[pathLen++] = current;
            current = parent[current];
        }
        
        // Rotayı ters çevir
        state->routeLength = pathLen;
        for (int i = 0; i < pathLen; i++) {
            state->routePath[i] = path[pathLen - 1 - i];
        }
        
        state->showRouteWindow = true;
    } else {
        state->routeLength = 0;
        state->showRouteWindow = false;
    }
}

// --- Ana UI Çizimi ---
void DrawMainUI(Graph* graph, UIState* state, Node* bst) {
    static bool startHovered[LINE_COUNT] = {false};
    static bool endHovered[LINE_COUNT] = {false};
    
    // Arkaplan gradyanı
    DrawRectangleGradientV(0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, 
                          (Color){20, 20, 40, 255}, (Color){40, 40, 80, 255});
    
    // Başlık
    const char* mainTitle = "ISTANBUL METRO ROTA PLANLAYICI";
    int mainTitleWidth = MeasureText(mainTitle, 32);
    DrawText(mainTitle, (MAIN_WINDOW_WIDTH - mainTitleWidth) / 2, 25, 32, WHITE);
    
    // Sol Panel - Başlangıç Durağı
    int leftPanelX = 50;
    int panelY = 90;
    
    DrawText("BASLANGIC DURAGI", leftPanelX, panelY, 20, (Color){200, 200, 255, 255});
    DrawLineCircles(leftPanelX + CIRCLE_RADIUS + 10, panelY + 60, &state->selectedStartLine, startHovered);
    DrawStopDropdown(leftPanelX, panelY + 180, 350, graph, state->selectedStartLine,
                     &state->selectedStartStopId, state->selectedStartName,
                     &state->showStartDropdown, &state->startDropdownScroll);
    
    // Sağ Panel - Bitiş Durağı
    int rightPanelX = MAIN_WINDOW_WIDTH - 400;
    
    DrawText("BITIS DURAGI", rightPanelX, panelY, 20, (Color){200, 200, 255, 255});
    DrawLineCircles(rightPanelX + CIRCLE_RADIUS + 10, panelY + 60, &state->selectedEndLine, endHovered);
    DrawStopDropdown(rightPanelX, panelY + 180, 350, graph, state->selectedEndLine,
                     &state->selectedEndStopId, state->selectedEndName,
                     &state->showEndDropdown, &state->endDropdownScroll);
    
    // Orta Panel - Seçim Özeti
    int centerX = MAIN_WINDOW_WIDTH / 2;
    int summaryY = 480;
    
    DrawRectangle(centerX - 200, summaryY, 400, 100, (Color){30, 30, 50, 200});
    DrawRectangleLinesEx((Rectangle){centerX - 200, summaryY, 400, 100}, 2, (Color){80, 80, 120, 255});
    
    char startText[150], endText[150];
    sprintf(startText, "Baslangic: %s", state->selectedStartName[0] ? state->selectedStartName : "Secilmedi");
    sprintf(endText, "Bitis: %s", state->selectedEndName[0] ? state->selectedEndName : "Secilmedi");
    
    DrawText(startText, centerX - 180, summaryY + 20, 16, WHITE);
    DrawText(endText, centerX - 180, summaryY + 50, 16, WHITE);
    
    // Alt Butonlar
    int btnY = 620;
    int btnWidth = 180;
    int btnHeight = 45;
    int btnSpacing = 25;
    int totalBtnWidth = 4 * btnWidth + 3 * btnSpacing;
    int btnStartX = (MAIN_WINDOW_WIDTH - totalBtnWidth) / 2;
    
    // Dijkstra Butonu
    Rectangle dijkstraBtn = {btnStartX, btnY, btnWidth, btnHeight};
    bool dijkstraHover = IsMouseOverRect(dijkstraBtn.x, dijkstraBtn.y, dijkstraBtn.width, dijkstraBtn.height);
    DrawRectangleRounded(dijkstraBtn, 0.3f, 8, dijkstraHover ? (Color){0, 150, 80, 255} : (Color){0, 120, 60, 255});
    DrawText("En Kisa Sure", btnStartX + 30, btnY + 15, 18, WHITE);
    
    if (dijkstraHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CalculateRouteForUI(graph, state, true);
    }
    
    // BFS Butonu
    Rectangle bfsBtn = {btnStartX + btnWidth + btnSpacing, btnY, btnWidth, btnHeight};
    bool bfsHover = IsMouseOverRect(bfsBtn.x, bfsBtn.y, bfsBtn.width, bfsBtn.height);
    DrawRectangleRounded(bfsBtn, 0.3f, 8, bfsHover ? (Color){0, 100, 180, 255} : (Color){0, 80, 150, 255});
    DrawText("En Az Durak", bfsBtn.x + 35, btnY + 15, 18, WHITE);
    
    if (bfsHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CalculateRouteForUI(graph, state, false);
    }
    
    // Temizle Butonu
    Rectangle clearBtn = {btnStartX + 2 * (btnWidth + btnSpacing), btnY, btnWidth, btnHeight};
    bool clearHover = IsMouseOverRect(clearBtn.x, clearBtn.y, clearBtn.width, clearBtn.height);
    DrawRectangleRounded(clearBtn, 0.3f, 8, clearHover ? (Color){180, 140, 0, 255} : (Color){150, 120, 0, 255});
    DrawText("Temizle", clearBtn.x + 55, btnY + 15, 18, WHITE);
    
    if (clearHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state->selectedStartLine = -1;
        state->selectedEndLine = -1;
        state->selectedStartStopId = -1;
        state->selectedEndStopId = -1;
        state->selectedStartName[0] = '\0';
        state->selectedEndName[0] = '\0';
        state->routeLength = 0;
        state->showRouteWindow = false;
    }
    
    // Çıkış Butonu
    Rectangle exitBtn = {btnStartX + 3 * (btnWidth + btnSpacing), btnY, btnWidth, btnHeight};
    bool exitHover = IsMouseOverRect(exitBtn.x, exitBtn.y, exitBtn.width, exitBtn.height);
    DrawRectangleRounded(exitBtn, 0.3f, 8, exitHover ? (Color){180, 50, 50, 255} : (Color){150, 40, 40, 255});
    DrawText("Cikis", exitBtn.x + 70, btnY + 15, 18, WHITE);
    
    if (exitHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CloseWindow();
    }
    
    // Alt bilgi
    DrawText("Istanbul Metro Rota Planlayici v2.0 | Raylib GUI", 20, MAIN_WINDOW_HEIGHT - 30, 14, (Color){120, 120, 150, 255});
    
    // Rota penceresi (en üstte çizilmeli)
    if (state->showRouteWindow) {
        DrawRouteWindow(graph, state);
    }
}
