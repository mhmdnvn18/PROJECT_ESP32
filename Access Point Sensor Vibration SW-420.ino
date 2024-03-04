#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

#define SW420_PIN 2 // Define the pin to which the SW-420 sensor is connected

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(SW420_PIN, INPUT); // Initialize the SW-420 sensor pin

  // Setup Access Point
  WiFi.softAP("ESP32-AP", "password");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Handle root URL request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<H1>TUGAS MIKROPROSESSOR</H1>";
    html += "<H1>SW-420 SENSOR STATUS</H1>";
    html += "<HR>";
    html += "<H2 id='sw420Status'>Loading...</H2>";
    html += "<h3> Anggota Kelompok :</h3>";
    html += "<p>1. 20210120004_Muhammad Novian</p>";
    html += "<p>2. 20210120050_Muhammad Aji</p>";
    html += "<p>3. 20210120069_Muhammad Andi</p>";

    html += "<script>";
    html += "setInterval(function() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      document.getElementById('sw420Status').innerHTML = 'SW-420 Status: ' + this.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/getSW420Status', true);";
    html += "  xhttp.send();";
    html += "}, 1000);";
    html += "</script>";

    request->send(200, "text/html", html);
  });

  // Handle the SW-420 sensor status
  server.on("/getSW420Status", HTTP_GET, [](AsyncWebServerRequest *request) {
    int sw420Status = digitalRead(SW420_PIN);
    String statusText = (sw420Status == HIGH) ? "Vibration Detected" : "No Vibration Detected";
    request->send(200, "text/plain", statusText);
  });

  // Start server
  server.begin();
}

void loop() {
  // No additional code in the loop function
}
