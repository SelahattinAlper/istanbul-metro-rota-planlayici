

Node* createNode(const char* name, int id);


Node* insert_bst(Node* root, const char* name, int id);


int search_bst(Node* root, const char* name);


// Min-Heap Fonksiyonları
MinHeap* createMinHeap(int capacity);
void swap_heap_nodes(HeapNode* a, HeapNode* b);
void min_heapify(MinHeap* minHeap, int idx);
HeapNode extract_min(MinHeap* minHeap);
void decrease_key(MinHeap* minHeap, int stop_id, long long new_distance);
int is_in_min_heap(MinHeap* minHeap, int stop_id);

// Dijkstra Algoritması
void print_path(Graph* graph, int end_id, const int parent[]); // Yardımcı
long long dijkstra_shortest_time(Graph* graph, int start_id, int end_id, int* out_parent);


// Graf Yükleme
void parse_and_build_graph(const char* filename, Graph* graph);
void add_edge(Stop* stop, int target_id, int duration, const char* line_name, int congestion_score, int is_closed);

// BFS
int bfs_min_transfers(Graph* graph, int start_id, int end_id, int* out_parent);

//Kuyruk Fonksiyonları
int is_queue_empty();
void enqueue(int stop_id);
int dequeue();