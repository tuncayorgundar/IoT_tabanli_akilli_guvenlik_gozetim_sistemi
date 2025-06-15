# IoT_tabanli_akilli_guvenlik_gozetim_sistemi

Bu proje, Wokwi platformunda simüle edilmiş, ESP32 tabanlı bir IoT güvenlik sistemidir. Sistem, bir PIR sensörü ile ortamdaki hareketi algılar ve yalnızca hareket tespit edildiğinde bir ultrasonik sensörle mesafeyi ölçer. Belirlenen mesafenin (100 cm) altına düşüldüğünde, görsel bir uyarı olarak LED yanıp sönmeye başlar. Tüm mesafe verileri MQTT protokolü aracılığıyla anlık olarak bir Node-RED paneline gönderilir; bu panel üzerinden hem veriler bir gösterge ile izlenebilir hem de LED uyarısı uzaktan kontrol edilebilir.
