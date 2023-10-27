#include <Wire.h>
#include <RTClib.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

RTC_DS3231 rtc;  // Inisialisasi objek RTC
const int SDA_PIN = 21; // Pin GPIO untuk SDA
const int SCL_PIN = 22; // Pin GPIO untuk SCL

const char* ssid = "HOKYA HOKYA";
const char* password = "noconcibaotroshijos";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // Inisialisasi komunikasi I2C
  rtc.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Koneksi WiFi...");
    Serial.print("Alamat IP ESP32: ");
    Serial.println(WiFi.localIP());
  }
  
  Serial.println("Koneksi WiFi berhasil");
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String timeStr = getFormattedTime();
    request->send(200, "text/html", timeStr);
  });
  
  server.begin();
}

void loop() {
  // Baca waktu dari RTC
  DateTime now = rtc.now();

  // Tunggu 1 detik
  delay(1000);
}

String getFormattedTime() {
  DateTime now = rtc.now();
  char buffer[50];
  sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  return String(buffer);
}
