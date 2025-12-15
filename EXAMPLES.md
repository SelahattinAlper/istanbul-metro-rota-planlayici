# Kullanım Örnekleri

Bu doküman, İstanbul Metro Rota Planlayıcı'nın farklı kullanım senaryolarını gösterir.

## Örnek 1: Anadolu Yakasından Avrupa Yakasına

### Senaryo: Kadıköy'den Taksim'e

```
--- ROTA PLANLAYICIYA HOS GELDINIZ ---
Baslangic duragini girin (Ornek: Kadikoy): Kadikoy
Hedef duragi girin (Ornek: Dudullu_M8): Taksim
Secilen Rota: Kadikoy (1) -> Taksim (204)

Lutfen bir islem seciniz:
1-En Kisa Sure Hesaplama (Dijkstra)
2-En Az Durak Hesaplama (BFS)
3-Yeni Rota Belirle
4-Cikis
Seciminiz: 1

--- EN KISA SURE ROTASI (DIJKSTRA) ---
Toplam Sure: 24 dakika
Rota: Kadikoy -> AyrilikCesmesi(M4) -> AyrilikCesmesi(Marmaray) -> 
      Uskudar(Marmaray) -> Sirkeci -> Yenikapi(Marmaray) -> 
      Yenikapi(M2) -> Vezneciler -> Halic -> Sishane -> Taksim
```

**Açıklama**: Marmaray kullanarak Boğaz'ı geçen en hızlı rota.

## Örnek 2: Avrupa Yakası İçi Ulaşım

### Senaryo: Yenikapi'dan Atatürk Havalimanı'na

```
Baslangic duragini girin: Yenikapi(M1A)
Hedef duragi girin: AtaturkHavalimani
Secilen Rota: Yenikapi(M1A) (300) -> AtaturkHavalimani (317)

Seciminiz: 1

--- EN KISA SURE ROTASI (DIJKSTRA) ---
Toplam Sure: 36 dakika
Rota: Yenikapi(M1A) -> Aksaray -> Emniyet-Fatih -> Topkapi-Ulubatli -> 
      Bayrampasa-Maltepe -> Sagmalcilar -> Kocatepe -> Otogar -> 
      Terazidere -> Davutpasa-YTU -> Merter -> Zeytinburnu -> 
      Bakirkoy-Incirli(M1A) -> Bahcelievler -> Atakoy-Sirinevler -> 
      Yenibosna(M1A) -> DTM-IstanbulFuarMerkezi -> AtaturkHavalimani
```

**Açıklama**: M1A hattı üzerinde direkt bağlantı.

## Örnek 3: En Az Aktarma

### Senaryo: Minimal Hat Değişikliği

```
Baslangic duragini girin: Kadikoy
Hedef duragi girin: Taksim
Secilen Rota: Kadikoy (1) -> Taksim (204)

Seciminiz: 2

--- EN AZ AKTARMA ROTASI (BFS) ---
Toplam Durak: 10
Toplam Aktarma: 3
Rota: Kadikoy -> AyrilikCesmesi(M4) -> [AKTARMA: M4 -> Marmaray] -> 
      AyrilikCesmesi(Marmaray) -> Uskudar(Marmaray) -> Sirkeci -> 
      Yenikapi(Marmaray) -> [AKTARMA: Marmaray -> M2] -> 
      Yenikapi(M2) -> Vezneciler -> Halic -> Sishane -> 
      [AKTARMA: M2 -> F1] -> Taksim
```

**Açıklama**: BFS algoritması en az aktarmalı rotayı bulur.

## Örnek 4: Anadolu Yakası İçi

### Senaryo: Üsküdar'dan Ümraniye'ye

```
Baslangic duragini girin: Uskudar(M5)
Hedef duragi girin: Umraniye
Secilen Rota: Uskudar(M5) (100) -> Umraniye (114)

Seciminiz: 1

--- EN KISA SURE ROTASI (DIJKSTRA) ---
Toplam Sure: 28 dakika
Rota: Uskudar(M5) -> Fistikagaci -> Baglarbaşi -> Altunizade -> 
      Kısıklı -> Bulgurlu -> Ümraniye(M5)
```

**Açıklama**: M5 hattı üzerinde direkt ulaşım.

## Örnek 5: Havalimanı Transferi

### Senaryo: Yeni Havalimanı'na Ulaşım

```
Baslangic duragini girin: Taksim
Hedef duragi girin: IstanbulHavalimani
Secilen Rota: Taksim (204) -> IstanbulHavalimani (950)

Seciminiz: 1

--- EN KISA SURE ROTASI (DIJKSTRA) ---
Toplam Sure: 45 dakika
Rota: Taksim -> Osmanbey -> Sisli-Mecidiyekoy -> Gayrettepe -> 
      4.Levent -> Levent -> ... -> Kagithane -> ... -> 
      IstanbulHavalimani
```

**Açıklama**: M11 hattı ile yeni havalimanına direkt bağlantı.

## Örnek 6: Hatalı Durak İsmi

### Senaryo: Bulunamayan Durak

```
Baslangic duragini girin: KadikoyYanlisIsim
Hedef duragi girin: Taksim

Hata: Baslangic veya hedef durak bulunamadi. 
Varsayilan (1 -> 109) kullaniliyor.
Secilen Rota: Kadikoy (1) -> Dudullu_M8 (109)
```

**Açıklama**: Yanlış durak ismi girildiğinde varsayılan rota kullanılır.

## İpuçları

### Durak İsimleri

- Durak isimlerini tam ve doğru yazın
- Alt çizgi kullanılan duraklar: `Dudullu_M8`, `4_Levent`
- Parantez içeren duraklar: `Yenikapi(M1A)`, `Uskudar(M5)`

### Hat Değişiklikleri

- Bazı duraklar birden fazla hat üzerindedir
- Aktarma noktalarında hat kodu belirtin: `Yenikapi(M1A)` veya `Yenikapi(M2)`

### Süre vs Aktarma

- **Dijkstra (Seçenek 1)**: En kısa süre ama daha çok aktarma olabilir
- **BFS (Seçenek 2)**: En az aktarma ama süre daha uzun olabilir

### Yoğunluk Faktörü

Program, yoğun hatlardaki yolculuk süresini otomatik olarak artırır:
- Yoğunluk Skoru 1: Normal süre
- Yoğunluk Skoru 5: %80 daha uzun süre

## Sık Kullanılan Rotalar

| Başlangıç | Hedef | En Hızlı Hat | Süre (dk) |
|-----------|-------|--------------|-----------|
| Kadıköy | Taksim | Marmaray + M2 | ~24 |
| Yenikapi | Havalimanı (Eski) | M1A | ~36 |
| Taksim | Havalimanı (Yeni) | M2 + M11 | ~45 |
| Üsküdar | Ümraniye | M5 | ~28 |
| Sişli | Kadıköy | M2 + Marmaray + M4 | ~30 |

## Gelişmiş Kullanım

### Aynı Rotayı Farklı Yöntemlerle Karşılaştırma

```
1. Dijkstra ile rota hesapla (Seçenek 1)
2. Sonucu not et
3. Aynı durakları tekrar seç (Seçenek 3)
4. BFS ile hesapla (Seçenek 2)
5. Sonuçları karşılaştır
```

### Alternatif Rotalar

Program her seferinde bir rota önerir. Alternatif rotalar için:
- Ara duraklara bölerek hesaplama yapın
- Farklı aktarma noktaları deneyin

---

**Not**: Bu örnekler mevcut metro.csv dosyasındaki verilere dayanmaktadır. 
Yeni hatlar eklendiğinde veya mevcut hatlar güncellendiğinde sonuçlar değişebilir.
