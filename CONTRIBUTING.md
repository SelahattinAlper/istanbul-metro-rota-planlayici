# Katkıda Bulunma Rehberi

İstanbul Metro Rota Planlayıcı projesine katkıda bulunmayı düşündüğünüz için teşekkür ederiz! Bu doküman, projeye nasıl katkı yapabileceğiniz konusunda size rehberlik edecektir.

## Başlamadan Önce

1. Projeyi fork edin
2. Repository'yi yerel makinenize klonlayın:
   ```bash
   git clone https://github.com/KULLANICI_ADINIZ/istanbul-metro-rota-planlayici.git
   cd istanbul-metro-rota-planlayici
   ```
3. Projeyi derleyin ve çalıştığından emin olun:
   ```bash
   make
   ./metro_planner
   ```

## Geliştirme Süreci

### 1. Yeni Bir Branch Oluşturun

```bash
git checkout -b feature/yeni-ozellik-adi
```

Branch isimlendirme önerileri:
- `feature/` - Yeni özellikler için
- `fix/` - Hata düzeltmeleri için
- `docs/` - Dokümantasyon güncellemeleri için
- `refactor/` - Kod iyileştirmeleri için

### 2. Kod Standartları

- **Kod Stili**: Mevcut kodun stiline uygun yazın
- **Yorumlar**: Karmaşık algoritmaları ve önemli kararları açıklayın
- **Değişken İsimlendirme**: Anlamlı ve açıklayıcı isimler kullanın
- **Türkçe**: Kullanıcıya yönelik mesajlar Türkçe olmalıdır

### 3. Derleme ve Test

Değişikliklerinizi yaptıktan sonra:

```bash
# Temiz bir build yapın
make clean
make

# Programı test edin
./metro_planner
```

Derlemede uyarı olmamalıdır:
```bash
make clean && make 2>&1 | grep -i warning
```

### 4. Commit Mesajları

İyi commit mesajları yazın:

```
Kısa başlık (50 karakterden az)

Detaylı açıklama (isteğe bağlı):
- Yapılan değişiklikler
- Değişikliğin nedeni
- Etkilenen alanlar
```

Örnekler:
- ✅ "Dijkstra algoritmasında bellek sızıntısı düzeltildi"
- ✅ "M11 hattı için durak bilgileri eklendi"
- ❌ "düzeltme"
- ❌ "update"

### 5. Pull Request Oluşturma

1. Değişikliklerinizi push edin:
   ```bash
   git push origin feature/yeni-ozellik-adi
   ```

2. GitHub'da Pull Request oluşturun

3. PR açıklamasında şunları belirtin:
   - Yapılan değişikliklerin özeti
   - Hangi sorunu çözdüğünüz veya hangi özelliği eklediğiniz
   - Test edildiğini gösterecek ekran görüntüleri veya çıktılar

## Katkı Önerileri

### Başlangıç Seviyesi

- Dokümantasyon güncellemeleri
- README'ye örnekler ekleme
- Hata mesajlarını iyileştirme
- Yorum ekleme
- `.gitignore` güncellemeleri

### Orta Seviye

- Yeni metro hatları ekleme
- Input validasyonu geliştirme
- Hata yakalama mekanizmaları
- Performans iyileştirmeleri
- Test senaryoları ekleme

### İleri Seviye

- Yeni rota bulma algoritmaları
- Grafik arayüz ekleme
- Web API geliştirme
- Gerçek zamanlı veri entegrasyonu
- Bellek optimizasyonları

## Metro Verisi Güncelleme

`metro.csv` dosyasını güncellerken:

1. Format tutarlılığını koruyun:
   ```
   id,isim,hat,komsular
   300,Yenikapi(M1A),M1A,301-2;200-5
   ```

2. Durak ID'lerinin benzersiz olduğundan emin olun

3. Komşu durak bağlantılarının karşılıklı olduğunu kontrol edin

4. Süre bilgilerinin gerçekçi olduğunu doğrulayın (dakika cinsinden)

## Sorun Bildirme

Bir hata bulduysanız veya öneriniz varsa:

1. Önce mevcut issue'lara bakın
2. Yeni bir issue açın ve şunları belirtin:
   - Sorunun açık bir tanımı
   - Sorunu yeniden oluşturma adımları
   - Beklenen davranış vs gerçekleşen davranış
   - Sistem bilgileri (OS, derleyici versiyonu)

## Kod İnceleme

Tüm Pull Request'ler incelenecektir. İnceleme sürecinde:

- Kod kalitesi kontrol edilir
- Fonksiyonellik test edilir
- Dokümantasyon yeterliliği değerlendirilir
- Performans etkileri göz önünde bulundurulur

## İletişim

- Issue'lar üzerinden tartışma
- Pull Request'lerde yorum
- Genel sorular için repository'nin issue bölümü

## Lisans

Katkılarınızı projenin mevcut lisansı altında sunmuş olursunuz.

---

Teşekkürler! 🚇
