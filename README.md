# IoT_tabanli_akilli_guvenlik_gozetim_sistemi

IoT Tabanlı Akıllı Güvenlik ve Gözetim Sistemi
![alt text](https://img.shields.io/badge/Platform-Wokwi-blueviolet)

![alt text](https://img.shields.io/badge/Board-ESP32-E7352C.svg)

![alt text](https://img.shields.io/badge/Protocol-MQTT-orange.svg)

![alt text](https://img.shields.io/badge/Dashboard-Node--RED-red.svg)
Bu proje, Necmettin Erbakan Üniversitesi Bilgisayar Mühendisliği Bölümü'nün "Nesnelerin İnterneti" dersi kapsamında geliştirilmiştir. Proje, bir ortamdaki hareketi algılayarak mesafeyi ölçen, tehlike anında görsel bir uyarı veren ve tüm bu verilerin Node-RED paneli üzerinden anlık olarak izlenip kontrol edilebildiği bir akıllı güvenlik sistemi simülasyonudur.
🏛️ Proje Mimarisi
Sistem, sensörlerden gelen verileri ESP32 ile işler, MQTT protokolü aracılığıyla buluttaki bir broker'a iletir ve son olarak Node-RED platformunda görselleştirip yönetir.
+---------------+      +-------+      +----------------+      +-----------------+      +-------------+
|    Sensörler  |  ->  | ESP32 |  ->  | Wokwi (Wi-Fi)  |  ->  |  MQTT Broker    |  ->  |   Node-RED  |
| (PIR, HC-SR04)|      +-------+      +----------------+      | (test.mosquitto)|      | (Dashboard) |
+---------------+                                           +-----------------+      +-------------+
        ^                                                            |                      |
        |------------------------------------------------------------+----------------------+
                                     (LED Kontrolü)
Use code with caution.
🚀 Temel Özellikler
Hareket Algılama: PIR sensörü ile ortamdaki hareketleri anlık olarak tespit eder.
Koşullu Mesafe Ölçümü: Sadece hareket algılandığında HC-SR04 ultrasonik sensör ile en yakın nesnenin mesafesini ölçer.
Gerçek Zamanlı Veri İletimi: Ölçülen mesafe verisi, benzersiz bir MQTT topic'i (<ÖĞRENCİ_NO>/mesafe) üzerinden test.mosquitto.org broker'ına anlık olarak yayınlanır.
Görsel Uyarı Sistemi: Hareket algılandığında ve mesafe 100 cm'den az olduğunda, LED 300ms aralıklarla yanıp sönerek uyarı verir.
Uzaktan Kontrol: Node-RED paneli üzerindeki bir buton aracılığıyla, yanıp sönen LED uyarısı uzaktan açılıp kapatılabilir.
Etkileşimli Dashboard: Node-RED üzerinde oluşturulan panelde, mesafe verisi bir "gauge" (gösterge) ile anlık olarak görüntülenir.
🛠️ Kullanılan Teknolojiler ve Donanımlar
Donanım (Wokwi Simülasyonu)
Geliştirme Kartı: ESP32
Sensörler: PIR Hareket Sensörü, HC-SR04 Ultrasonik Mesafe Sensörü
Çıkış Birimi: LED, Direnç
Diğer: Breadboard
Yazılım ve Platformlar
Simülasyon Ortamı: Wokwi
Programlama: Arduino C/C++ (.ino)
İletişim Protokolü: MQTT
MQTT Broker: test.mosquitto.org:1883
Veri Görselleştirme ve Kontrol: Node-RED
Node-RED Eklentisi: node-red-dashboard
⚙️ Kurulum ve Çalıştırma
Bu projenin simülasyonunu çalıştırmak ve sistemi test etmek için aşağıdaki adımları izleyin.
Bölüm 1: Wokwi Simülasyonu
Projeyi Klonlayın: Bu repoyu yerel makinenize indirin.
Kodu Düzenleyin:
sketch.ino dosyasını bir metin düzenleyici ile açın.
MQTT topic'inin size özel olması için aşağıdaki satırı bulun ve YOUR_STUDENT_NO kısmını kendi öğrenci numaranızla değiştirin:
// Bu satırı bulun ve değiştirin:
const char* mesafe_topic = "YOUR_STUDENT_NO/mesafe";
Use code with caution.
C++
Simülasyonu Başlatın:
Wokwi platformunda bu projeye ait simülasyonu açın. (Not: Wokwi, proje dosyalarını doğrudan yükleyerek değil, tarayıcı tabanlı çalışır. Bu nedenle proje dosyalarını Wokwi editörüne kopyalayıp yapıştırmanız gerekebilir.)
sketch.ino, diagram.json dosyalarının içeriğini Wokwi'deki ilgili sekmelere yapıştırın.
Simülasyonu başlatmak için yeşil "Play" (▶) düğmesine basın.
Bölüm 2: Node-RED Yapılandırması
Node-RED'in bilgisayarınızda kurulu ve çalışıyor olması gerekmektedir. Resmi Kurulum Rehberi
Node-RED Dashboard Eklentisini Yükleyin:
Eğer yüklü değilse, terminali açın ve Node-RED kullanıcı dizininize gidin (genellikle ~/.node-red).
Aşağıdaki komutu çalıştırın ve Node-RED'i yeniden başlatın:
npm install node-red-dashboard
Use code with caution.
Bash
Node-RED Akışını (Flow) İçe Aktarın:
Tarayıcınızda Node-RED arayüzünü açın (genellikle http://127.0.0.1:1880).
Sağ üst köşedeki menü (☰) ikonuna tıklayıp Import seçeneğini seçin.
select a file to import düğmesine tıklayarak bu projedeki OgrNo-Flows.json dosyasını seçin.
Import düğmesine basarak akışı içe aktarın.
Akışı Dağıtın (Deploy):
Sağ üst köşedeki kırmızı Deploy düğmesine basarak akışı aktif hale getirin.
Sistemi canlı olarak izlemek için dashboard arayüzünü açın: http://127.0.0.1:1880/ui
Artık Wokwi simülasyonundaki sensörleri tetiklediğinizde, verilerin Node-RED panelinize anlık olarak yansıdığını görebilirsiniz.
📁 Proje Dosyaları
sketch.ino: ESP32 kartı üzerinde çalışan, sensör verilerini okuyan, LED'i kontrol eden ve MQTT iletişimini yöneten ana kod.
diagram.json: Wokwi simülasyonundaki donanım bileşenlerini ve bağlantılarını tanımlayan yapılandırma dosyası.
OgrNo-Flows.json: Node-RED'de kullanılacak olan, MQTT dinleme, dashboard göstergesi ve kontrol butonu düğümlerini içeren akış dosyası.
README.md: Bu dosya; projenin detaylı açıklaması ve kurulum rehberi.
