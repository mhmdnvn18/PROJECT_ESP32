#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

#define MQ2_PIN A0 // Define the analog pin to which the MQ-2 gas sensor is connected

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  analogReadResolution(12); // Set the ADC resolution to 12 bits (4096 levels)

  // Setup Access Point
  WiFi.softAP("ESP32-AP", "password");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Handle root URL request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<H1>TUGAS MIKROPROSESSOR</H1>";
    html += "<H1>MQ-2 GAS SENSOR STATUS</H1>";
    html += "<HR>";
    html += "<H2 id='mq2Status'>Loading...</H2>";
    html += "<h3> Anggota Kelompok :</h3>";
    html += "<p>1. 20210120004_Muhammad Novian</p>";
    html += "<p>2. 20210120050_Muhammad Aji</p>";
    html += "<p>3. 20210120069_Muhammad Andi</p>";

    html += "<script>";
    html += "setInterval(function() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      document.getElementById('mq2Status').innerHTML = 'MQ-2 Status: ' + this.responseText;";
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/getMQ2Status', true);";
    html += "  xhttp.send();";
    html += "}, 1000);";
    html += "</script>";

    request->send(200, "text/html", html);
  });

  // Handle the MQ-2 gas sensor status
  server.on("/getMQ2Status", HTTP_GET, [](AsyncWebServerRequest *request) {
    int mq2Value = analogRead(MQ2_PIN); // Read the analog value from the MQ-2 gas sensor
    String statusText;

    // Customize the logic based on your MQ-2 sensor's behavior
    if (mq2Value > 1000) {
      statusText = "Gas Detected"; // Adjust the threshold value as needed
    } else {
      statusText = "No Gas Detected";
    }

    request->send(200, "text/plain", statusText);
  });

  // Start server
  server.begin();
}

void loop() {
  // No additional code in the loop function
}
