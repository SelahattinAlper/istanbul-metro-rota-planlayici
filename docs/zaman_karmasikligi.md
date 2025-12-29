# 📊 Istanbul Metro Rota Planlayıcı - Zaman Karmaşıklığı Analizi

Bu dokümanda, projedeki tüm dosyalar ve fonksiyonların zaman karmaşıklıkları **Big-O notasyonuyla** gösterilmiştir.

## 📌 Notasyon Açıklamaları

| Sembol | Açıklama |
|--------|----------|
| **N** | Toplam durak sayısı (`MAX_STOPS` = 1500) |
| **E** | Toplam kenar sayısı (komşuluk bağlantıları) |
| **H** | BST ağaç yüksekliği (ortalama `log N`, en kötü `N`) |

---

## 📁 **bst_tree.c** - İkili Arama Ağacı (BST)

| Fonksiyon | Zaman Karmaşıklığı | Açıklama |
|-----------|-------------------|----------|
| `createNode()` | **O(1)** | Sabit zamanlı bellek ayırma ve değer atama |
| `insert_bst()` | **O(H)** → Ortalama **O(log N)**, En kötü **O(N)** | Rekursif ekleme, ağaç dengesine bağlı |
| `search_bst()` | **O(H)** → Ortalama **O(log N)**, En kötü **O(N)** | Rekursif arama, ağaç dengesine bağlı |

---

## 📁 **min_heap.c** - Min-Heap (Öncelik Kuyruğu)

| Fonksiyon | Zaman Karmaşıklığı | Açıklama |
|-----------|-------------------|----------|
| `createMinHeap()` | **O(N)** | Kapasite boyutunda dizi oluşturma |
| `swap_heap_nodes()` | **O(1)** | Sabit zamanlı düğüm takası |
| `min_heapify()` | **O(log N)** | Aşağı doğru heap özelliği koruma |
| `extract_min()` | **O(log N)** | Minimum çıkarma + heapify |
| `decrease_key()` | **O(log N)** | Yukarı doğru heap-up işlemi |
| `is_in_min_heap()` | **O(1)** | Position dizisiyle sabit zamanlı kontrol |

---

## 📁 **graph_loader.c** - Graf Yükleme

| Fonksiyon | Zaman Karmaşıklığı | Açıklama |
|-----------|-------------------|----------|
| `add_edge()` | **O(1)** | Bağlı listenin başına ekleme (head insert) |
| `parse_and_build_graph()` | **O(N + E)** | CSV dosyasını 2 geçişle okuma, tüm durak ve kenarları oluşturma |
| `generate_random_congestion()` | **O(E)** | Tüm kenarları dolaşarak rastgele değer atama |

---

## 📁 **route_solver.c** - Rota Algoritmaları

| Fonksiyon | Zaman Karmaşıklığı | Açıklama |
|-----------|-------------------|----------|
| `print_path()` | **O(N)** | Rekursif yol yazdırma (en uzun yol N durak) |
| `calculate_dynamic_cost()` | **O(1)** | Sabit zamanlı maliyet hesaplama |
| `dijkstra_shortest_time()` | **O((N + E) log N)** | Dijkstra algoritması, min-heap ile |
| `is_queue_empty()` | **O(1)** | Kuyruk boşluk kontrolü |
| `enqueue()` | **O(1)** | Kuyruğa ekleme |
| `dequeue()` | **O(1)** | Kuyruktan çıkarma |
| `bfs_min_transfers()` | **O(N + E)** | BFS algoritması (level-order traversal) |

---

##  Özet Karşılaştırma Tablosu

| Dosya | En Önemli Fonksiyon | Karmaşıklık |
|-------|---------------------|-------------|
| `bst_tree.c` | `insert_bst()`, `search_bst()` | **O(log N)** ortalama |
| `min_heap.c` | `extract_min()`, `decrease_key()` | **O(log N)** |
| `graph_loader.c` | `parse_and_build_graph()` | **O(N + E)** |
| `route_solver.c` | `dijkstra_shortest_time()` | **O((N + E) log N)** |
| `route_solver.c` | `bfs_min_transfers()` | **O(N + E)** |

---

## 🔬 Algoritma Karmaşıklık Detayları

### Dijkstra Algoritması - O((N + E) log N)

```
dijkstra_shortest_time():
├── Başlatma: O(N) - dist[], parent[] dizileri
├── Heap oluşturma: O(N) - tüm düğümler heap'e eklenir
├── Ana döngü: O(N) iterasyon
│   ├── extract_min(): O(log N)
│   └── Her komşu için: O(degree(u))
│       └── decrease_key(): O(log N)
└── Toplam: O(N × log N) + O(E × log N) = O((N + E) log N)
```

### BFS Algoritması - O(N + E)

```
bfs_min_transfers():
├── Başlatma: O(N) - level[], parent[] dizileri
├── Ana döngü: Her düğüm en fazla 1 kez işlenir
│   ├── dequeue(): O(1)
│   └── Her komşu için: O(degree(u))
│       └── enqueue(): O(1)
└── Toplam: O(N) + O(E) = O(N + E)
```

### BST İşlemleri - O(H)

```
insert_bst() / search_bst():
├── Her adımda sola veya sağa gidilir
├── Dengeli ağaç: H = log N → O(log N)
├── Dengesiz ağaç (en kötü): H = N → O(N)
└── Bu projede: Alfabetik sıra riski var, AVL/RB-Tree önerilir
```

---

## 💡 Performans Notları

1. **Dijkstra Algoritması**: Min-heap kullanıldığı için karmaşıklık **O((N + E) log N)**, bu optimum performanstır. Fibonacci Heap ile O(N log N + E) elde edilebilir, ancak pratik uygulamalarda binary heap yeterlidir.

2. **BFS Algoritması**: Dizi tabanlı kuyruk kullanıldığı için **O(N + E)** karmaşıklığına sahiptir. Bu, ağırlıksız graflarda en kısa yol için optimal çözümdür.

3. **BST (İkili Arama Ağacı)**: Mevcut implementasyon dengeli değildir. Alfabetik sırada ekleme yapılırsa en kötü durumda **O(N)** olur. Performans iyileştirmesi için AVL veya Red-Black Tree kullanılabilir.

4. **Graf Temsili**: Komşuluk listesi (Adjacency List) kullanılmıştır. Bu sayede:
   - Alan karmaşıklığı: **O(N + E)**
   - Kenar sorgulama: **O(degree(v))**
   - Tüm kenarları gezme: **O(E)**

5. **Pratik Performans**: İstanbul metro sistemi için N ≈ 150 durak ve E ≈ 300 kenar olduğundan, tüm algoritmalar milisaniyeler içinde sonuç döndürür.

---

## 📈 Alan (Space) Karmaşıklığı

| Veri Yapısı | Alan Karmaşıklığı | Açıklama |
|-------------|-------------------|----------|
| Graf (Graph) | **O(N + E)** | Duraklar + Kenarlar |
| BST | **O(N)** | Her durak için bir düğüm |
| Min-Heap | **O(N)** | array + pos dizileri |
| Dijkstra dist[] | **O(N)** | Mesafe dizisi |
| BFS level[] | **O(N)** | Seviye dizisi |
| Kuyruk (BFS) | **O(N)** | En fazla N eleman |

**Toplam Alan Karmaşıklığı**: **O(N + E)**

---

*Bu doküman, Istanbul Metro Rota Planlayıcı projesinin algoritma karmaşıklık analizini içermektedir.*

*Son güncelleme: 29 Aralık 2025*
