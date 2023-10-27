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

DateTime currentDateTime;  // Variabel untuk menyimpan waktu dari RTC

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // Inisialisasi komunikasi I2C
  rtc.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Koneksi WiFi...");
  }
  
  Serial.println("Koneksi WiFi berhasil");
  Serial.print("Alamat IP ESP32: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // Ambil waktu terbaru dari RTC sebelum menampilkan halaman web
    currentDateTime = rtc.now();
    
    // Format waktu
    String timeStr = getFormattedTime(currentDateTime);
    
    // Buat halaman web
    String html = "<html>";
    html += "<head>";
    html += "<title>Waktu dari RTC DS3231</title>";
    html += "<meta http-equiv='refresh' content='1'>"; // Refresh halaman setiap 1 detik
    html += "</head>";
    html += "<body>";
    html += "<h1>Waktu dari RTC DS3231</h1>";
    html += "<p>Waktu sekarang: <span id='currentTime'>" + timeStr + "</span></p>";
    html += "<script>";
    html += "function updateClock() {";
    html += "var currentTime = new Date();";
    html += "var timeStr = currentTime.toISOString().slice(0, 19).replace('T', ' ');";
    html += "document.getElementById('currentTime').innerText = timeStr;";
    html += "}";
    html += "updateClock();";
    html += "setInterval(updateClock, 1000);"; // Atur interval pembaruan waktu setiap 1 detik
    html += "</script>";
    html += "</body>";
    html += "</html>";
    
    request->send(200, "text/html", html);
  });
  
  server.begin();
}

void loop() {
  // Tidak perlu membaca waktu dari RTC pada setiap siklus loop, karena waktu sudah diambil saat ada permintaan HTTP
  // Tunggu 1 detik
  delay(1000);
}

String getFormattedTime(DateTime dt) {
  char buffer[50];
  sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
  return String(buffer);
}
