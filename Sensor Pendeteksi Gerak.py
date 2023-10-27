#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

#define PIR_PIN 2 // Ganti dengan pin yang sesuai pada ESP32 Anda

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT); // Set pin PIR sebagai input

  // Setup Access Point
  WiFi.softAP("ESP32-AP", "password");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Handle root URL request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<H1>TUGAS MIKROPROSESSOR</H1>";
    html += "<H1>PIR MOTION SENSOR STATUS</H1>";
    html += "<HR>";
    html += "<H2 id='pirStatus'>Loading...</H2>";
    html += "<h3> Anggota Kelompok :</h3>";
    html += "<p>1. 20210120004_Muhammad Novian</p>";
    html += "<p>2. 20210120050_Muhammad Aji</p>";
    html += "<p>3. 20210120069_Muhammad Andi</p>";
    html += "<script>";
    html += "setInterval(function() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      document.getElementById('pirStatus').innerHTML = 'PIR Status: ' + this.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/getPIRStatus', true);";
    html += "  xhttp.send();";
    html += "}, 1000);";
    html += "</script>";

    request->send(200, "text/html", html);
  });

  // Handle PIR motion sensor status
  server.on("/getPIRStatus", HTTP_GET, [](AsyncWebServerRequest *request) {
    int pirValue = digitalRead(PIR_PIN); // Baca nilai dari sensor PIR
    String statusText;

    if (pirValue == HIGH) {
      statusText = "Motion Detected";
    } else {
      statusText = "No Motion Detected";
    }

    request->send(200, "text/plain", statusText);
  });

  // Start server
  server.begin();
}

void loop() {
  // Tidak ada kode tambahan dalam fungsi loop
}
