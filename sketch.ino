// Gerekli kütüphaneler ekleniyor
#include <WiFi.h>              // ESP32 için WiFi bağlantısı
#include <PubSubClient.h>      // MQTT protokolü ile haberleşme için
#include <NewPing.h>           // Ultrasonik sensör için daha kararlı ölçüm kütüphanesi

// WiFi ağı ve MQTT broker bilgileri
const char* ssid = "Wokwi-GUEST";                      // Wokwi varsayılan WiFi SSID
const char* password = "";                             // Şifresiz bağlantı
const char* mqtt_server = "test.mosquitto.org";        // Ücretsiz MQTT broker
const char* topic_mesafe = "22100011058/mesafe";       // Mesafe verisinin yayınlanacağı topic
const char* topic_led = "22100011058/led";             // Dashboard'daki butonun dinleneceği topic

// ESP32 GPIO pin tanımları
#define TRIG_PIN 5          // Ultrasonik sensör TRIG pini
#define ECHO_PIN 18         // Ultrasonik sensör ECHO pini
#define PIR_PIN 19          // PIR hareket sensörünün OUT pini
#define RED_LED 25          // RGB LED'in kırmızı bacağı
#define GREEN_LED 26        // RGB LED'in yeşil bacağı
#define BLUE_LED 27         // RGB LED'in mavi bacağı (bu projede kullanılmıyor)

// Ultrasonik sensör için nesne oluşturuluyor (maksimum mesafe: 400 cm)
NewPing sonar(TRIG_PIN, ECHO_PIN, 400);

// WiFi ve MQTT istemcileri oluşturuluyor
WiFiClient espClient;
PubSubClient client(espClient);

// LED'in kontrol edilebilir olup olmadığını belirleyen bayrak
bool ledDurum = true;

// WiFi bağlantısı kuruluyor
void setup_wifi() {
  delay(100);
  Serial.println("WiFi bağlanıyor...");
  WiFi.begin(ssid, password);    // WiFi ağına bağlan

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  // WiFi bağlantı durumu yazdırılıyor
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Bağlantısı başarılı!");
    Serial.println(WiFi.localIP()); // IP adresi konsola yazdırılıyor
  } else {
    Serial.println("\nWiFi Bağlantısı başarısız.");
  }
}

// MQTT üzerinden gelen mesajları işleyen fonksiyon
void callback(char* topic_led, byte* payload, unsigned int length) {
  String message;

  // Mesaj verisi karakter karakter okunup stringe çevriliyor
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Eğer "on" mesajı geldiyse LED kontrol aktif ediliyor
  if (message == "on") {
    ledDurum = true;
  }
  // Eğer "off" mesajı geldiyse RGB LED tamamen kapatılıyor
  else if (message == "off") {
    ledDurum = false;
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }
}

// MQTT broker ile bağlantı kurulmaya çalışılıyor
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client-11058")) {
      client.subscribe(topic_led); // LED kontrol topic'ine abone olunuyor
    } else {
      delay(5000); // Bağlantı başarısızsa 5 saniye bekle ve tekrar dene
    }
  }
}

// RGB LED'e renk atanmasını sağlayan fonksiyon
void setColor(bool r, bool g, bool b) {
  digitalWrite(RED_LED, r);     // Renk değerlerine göre LED'leri yak/söndür
  digitalWrite(GREEN_LED, g);
  digitalWrite(BLUE_LED, b);
}

// Başlangıç fonksiyonu
void setup() {
  Serial.begin(115200);             // Seri haberleşme başlatılıyor
  pinMode(PIR_PIN, INPUT);          // PIR sensör girişi
  pinMode(RED_LED, OUTPUT);         // RGB LED çıkışları
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  setup_wifi();                     // WiFi bağlantısı kuruluyor
  client.setServer(mqtt_server, 1883);      // MQTT broker ayarı
  client.setCallback(callback);             // MQTT mesaj geldiğinde çalışacak fonksiyon
}

// Ana döngü
void loop() {
  // MQTT bağlantısı yoksa yeniden bağlan
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // MQTT olaylarını işle

  // Sensörlerden veri okunuyor
  int mesafe = sonar.ping_cm();               // Ultrasonik mesafe ölçümü
  bool hareket = digitalRead(PIR_PIN);        // PIR sensör hareket algılama

  // Ölçüm değerleri seri portta yazdırılıyor
  Serial.print("Mesafe: ");
  Serial.print(mesafe);
  Serial.print(" cm | Hareket: ");
  Serial.println(hareket);

  // Mesafe MQTT üzerinden gönderiliyor
  if (mesafe > 0 && mesafe <= 400) {
    char payload[10];
    sprintf(payload, "%d", mesafe);               // Integer değeri stringe çevir
    client.publish(topic_mesafe, payload);        // MQTT ile yayınla
  }

  // LED kontrolü aktifse mesafeye ve harekete göre renk belirleniyor
  if (ledDurum) {
    if (mesafe > 0 && mesafe <= 100) {
      if (hareket) {
        // Tehlike anı: kırmızı LED yanıp söner
        setColor(1, 0, 0); delay(300);
        setColor(0, 0, 0); delay(300);
      } else {
        setColor(1, 0, 0);  // Sabit kırmızı (tehlike yok ama mesafe yakın)
      }
    } else if (mesafe > 100 && mesafe <= 200) {
      setColor(1, 1, 0); // Sarı (orta mesafe)
    } else if (mesafe > 200 && mesafe <= 400) {
      setColor(0, 1, 0); // Yeşil (güvenli mesafe)
    } else {
      setColor(0, 0, 0); // Mesafe geçersizse LED kapalı
    }
  } else {
    // Node-RED dashboard'dan gelen komut ile LED tamamen kapatılmışsa
    setColor(0, 0, 0);
  }

  delay(100); // Sistem yükünü azaltmak için kısa gecikme
}
