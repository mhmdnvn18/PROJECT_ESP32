// 5V + GND
// Trig Pin 13, Echo Pin 12

#include <Wire.h>
#include <Ultrasonic.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

Ultrasonic ultrasonic(13, 12); // Trig Pin, Echo Pin
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Setup Access Point
  WiFi.softAP("ESP32-AP", "password");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Handle root URL request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<H1>TUGAS MIKROPROSESSOR</H1>";
    html += "<H1>DISTANCE SENSOR READING</H1>";
    html += "<HR>";
    html += "<H2 id='distance'>Loading...</H2>";
    html += "<h3> Anggota Kelompok :</h3>";
    html += "<p>1. 20210120004_Muhammad Novian</p>";
    html += "<p>2. 20210120050_Muhammad Aji</p>";
    html += "<p>3. 20210120069_Muhammad Andi</p>";
    html += "<script>";
    html += "setInterval(function() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      document.getElementById('distance').innerHTML = 'Distance: ' + this.responseText + ' cm';";
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/getDistance', true);";
    html += "  xhttp.send();";
    html += "}, 1000);";
    html += "</script>";
    html += "</body></html>";

    request->send(200, "text/html", html);
  });

  // Handle AJAX request to get distance
  server.on("/getDistance", HTTP_GET, [](AsyncWebServerRequest *request) {
    float distance = ultrasonic.distanceRead();
    String distanceString = String(distance, 1);
    request->send(200, "text/plain", distanceString);
  });

  // Start server
  server.begin();
}

void loop() {
  // No additional code in the loop function
}
