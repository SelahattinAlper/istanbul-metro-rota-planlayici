# İstanbul Metro Rota Planlayıcı

İstanbul metro ağında iki durak arasında en optimal rotayı bulan bir C dili konsol uygulaması. Dijkstra ve BFS algoritmalarını kullanarak hem en kısa süre hem de en az aktarma seçenekleri sunar.

## Özellikler

- 🚇 **İstanbul Metro Ağı**: Tüm metro hatlarını içeren güncel durak bilgileri
- ⚡ **İki Optimizasyon Modu**:
  - **Dijkstra Algoritması**: En kısa sürede ulaşım
  - **BFS Algoritması**: En az aktarma ile ulaşım
- 🔍 **BST ile Hızlı Arama**: İkili arama ağacı kullanarak durak isimlerine göre hızlı arama
- 📊 **Dinamik Maliyet Hesaplama**: Yoğunluk ve kapalı yol bilgilerini dikkate alır
- 💻 **Kullanıcı Dostu Arayüz**: Basit ve anlaşılır konsol menüsü

## Gereksinimler

- GCC derleyicisi (veya başka bir C derleyicisi)
- Make (opsiyonel, derlemeyi kolaylaştırır)
- Linux/Unix tabanlı işletim sistemi veya Windows (MinGW ile)

## Derleme

### Make ile (Önerilen)

```bash
make
```

### Manuel Derleme

```bash
gcc main.c src/bst_tree.c src/graph_loader.c src/min_heap.c src/route_solver.c -I. -o metro_planner -lm
```

## Kullanım

Programı çalıştırmak için:

```bash
./metro_planner
```

### Örnek Kullanım

1. Program başladığında başlangıç ve hedef durak isimlerini girin:
   ```
   Baslangic duragini girin (Ornek: Kadikoy): Kadikoy
   Hedef duragi girin (Ornek: Taksim): Taksim
   ```

2. İstediğiniz hesaplama yöntemini seçin:
   - **1**: En kısa süre (Dijkstra)
   - **2**: En az aktarma (BFS)
   - **3**: Yeni rota belirle
   - **4**: Çıkış

## Proje Yapısı

```
.
├── main.c                 # Ana program dosyası
├── includes/
│   ├── structs.h         # Veri yapıları tanımları
│   └── algorithms.h      # Algoritma fonksiyon prototipleri
├── src/
│   ├── bst_tree.c        # İkili arama ağacı implementasyonu
│   ├── graph_loader.c    # Graf yükleme ve CSV parse işlemleri
│   ├── min_heap.c        # Min-heap veri yapısı (Dijkstra için)
│   └── route_solver.c    # Rota hesaplama algoritmaları
├── metro.csv             # İstanbul metro ağı verileri
└── Makefile              # Derleme yapılandırması
```

## Veri Formatı

`metro.csv` dosyası aşağıdaki formatta durak bilgilerini içerir:

```
id,isim,hat,komsular
300,Yenikapi(M1A),M1A,301-2;200-5;812-5
```

- **id**: Durak ID'si
- **isim**: Durak adı
- **hat**: Metro hattı kodu
- **komsular**: Komşu duraklar (format: id-süre;id-süre)

## Algoritmalar

### Dijkstra Algoritması
- En kısa süreyi bulmak için kullanılır
- Yoğunluk skorlarını dikkate alarak dinamik maliyet hesaplar
- Min-heap kullanarak O((V+E) log V) karmaşıklığında çalışır

### BFS (Breadth-First Search)
- En az durak/aktarma sayısını bulmak için kullanılır
- Hat değişikliklerini tespit eder
- O(V+E) karmaşıklığında çalışır

## Katkıda Bulunma

Projeye katkıda bulunmak isterseniz:

1. Repository'yi fork edin
2. Yeni bir branch oluşturun (`git checkout -b feature/yeniOzellik`)
3. Değişikliklerinizi commit edin (`git commit -am 'Yeni özellik eklendi'`)
4. Branch'inizi push edin (`git push origin feature/yeniOzellik`)
5. Pull Request oluşturun

Detaylı bilgi için [CONTRIBUTING.md](CONTRIBUTING.md) dosyasına bakınız.

## Dökümanlar

- **[README.md](README.md)**: Projeye genel bakış ve kurulum
- **[EXAMPLES.md](EXAMPLES.md)**: Detaylı kullanım örnekleri ve senaryolar
- **[CONTRIBUTING.md](CONTRIBUTING.md)**: Katkıda bulunma rehberi

## Lisans

Bu proje eğitim amaçlı geliştirilmiştir.

## İletişim

Sorularınız veya önerileriniz için issue açabilirsiniz.
