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

- **Git** (Raylib'i indirmek için gerekli)
- **CMake** (3.15 veya üzeri)
- **C Derleyici**: GCC (MinGW-w64), Clang veya MSVC
- **Raylib**: ✨ Otomatik olarak CMake ile indirilir (manuel kurulum gerekmez!)
- **IDE (Önerilen)**: CLion veya VS Code + CMake Tools eklentisi

## Derleme

### CLion ile

1. Projeyi CLion'da açın
2. CMake otomatik olarak yapılandırılacaktır
3. **Build > Build Project** veya `Ctrl+F9` ile derleyin
4. **Run > Run** veya `Shift+F10` ile çalıştırın

### VS Code ile

1. Gerekli eklentileri yükleyin:
   - **C/C++** (Microsoft)
   - **CMake Tools** (Microsoft)

2. Projeyi VS Code'da açın
3. `Ctrl+Shift+P` > "CMake: Configure" seçin
4. `Ctrl+Shift+B` ile derleyin veya F5 ile debug modda başlatın

### Komut Satırı ile

```bash
# Build klasörü oluştur ve yapılandır
mkdir build
cd build
cmake .. -G "MinGW Makefiles"  # Windows için
# veya
cmake ..  # Linux/macOS için

# Derle
cmake --build .

# Çalıştır
./IstanbulMetroRotaPlanlayici  # Linux/macOS
IstanbulMetroRotaPlanlayici.exe  # Windows
```

## Kullanım

Program çalıştırıldığında grafiksel arayüz açılır:

1. **Hat Seçimi**: Sol ve sağ panellerde metro hat dairelerine tıklayarak başlangıç ve bitiş hatlarını seçin
2. **Durak Seçimi**: Açılan dropdown listelerden durakları seçin
3. **Rota Hesaplama**:
   - **"En Kısa Süre"**: Dijkstra algoritması ile en kısa süreyi hesaplar
   - **"En Az Durak"**: BFS algoritması ile en az duraklı rotayı hesaplar
4. **Yoğunluk Güncelle**: Canlı trafik simülasyonu için yoğunluk değerlerini yeniler

## Proje Yapısı

```
IstanbulMetroRotaPlanlayici/
├── main.c                    # Ana program dosyası (UI başlatma)
├── CMakeLists.txt            # CMake yapılandırması
├── metro.csv                 # İstanbul metro ağı verileri
├── README.md                 # Bu dosya
│
├── includes/                 # Header dosyaları
│   ├── structs.h            # Veri yapıları (Graph, Node, Edge, etc.)
│   ├── algorithms.h         # Algoritma fonksiyon prototipleri
│   └── ui.h                 # UI yapıları ve fonksiyon prototipleri
│
├── src/                      # Kaynak dosyaları
│   ├── bst_tree.c           # İkili arama ağacı (BST) implementasyonu
│   ├── graph_loader.c       # Graf yükleme ve CSV parse işlemleri
│   ├── min_heap.c           # Min-heap veri yapısı (Dijkstra için)
│   ├── route_solver.c       # Dijkstra ve BFS algoritmaları
│   └── ui.c                 # Raylib tabanlı grafiksel arayüz
│
├── .vscode/                  # VS Code yapılandırması
│   ├── settings.json        # Editor ayarları
│   ├── c_cpp_properties.json # IntelliSense ayarları
│   ├── tasks.json           # Derleme görevleri
│   └── launch.json          # Debug/çalıştırma yapılandırması
│
├── .idea/                    # CLion yapılandırması (otomatik oluşturulur)
├── cmake-build-debug/        # CLion build klasörü (otomatik oluşturulur)
└── build/                    # VS Code/CLI build klasörü (otomatik oluşturulur)
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

## Lisans

Bu proje eğitim amaçlı geliştirilmiştir.

## İletişim

Sorularınız veya önerileriniz için issue açabilirsiniz.
