#include <Wire.h>
#include <DHT.h> // Include the DHT library
#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

#define DHTPIN 2     // Change to pin 2
#define DHTTYPE DHT11 // Define the DHT sensor type (DHT11 or DHT22)

DHT dht(DHTPIN, DHTTYPE); // Initialize the DHT sensor
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize the DHT sensor
  dht.begin();

  // Setup Access Point
  WiFi.softAP("ESP32-AP", "password");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Handle root URL request
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<H1>TUGAS MIKROPROSESSOR</H1>";
    html += "<H1>SENSOR READINGS</H1>";
    html += "<HR>";
    html += "<H2 id='temperature'>Loading...</H2>";
    html += "<H2 id='humidity'>Loading...</H2>";
    html += "<h3> Anggota Kelompok :</h3>";
    html += "<p>1. 20210120004_Muhammad Novian</p>";
    html += "<p>2. 20210120050_Muhammad Aji</p>";
    html += "<p>3. 20210120069_Muhammad Andi</p>";
    html += "<script>";
    html += "setInterval(function() {";
    html += "  var xhttp = new XMLHttpRequest();";
    html += "  xhttp.onreadystatechange = function() {";
    html += "    if (this.readyState == 4 && this.status == 200) {";
    html += "      var data = JSON.parse(this.responseText);";
    html += "      document.getElementById('temperature').innerHTML = 'Temperature: ' + data.temperature + ' °C';";
    html += "      document.getElementById('humidity').innerHTML = 'Humidity: ' + data.humidity + '%';";
    html += "    }";
    html += "  };";
    html += "  xhttp.open('GET', '/getSensorData', true);";
    html += "  xhttp.send();";
    html += "}, 1000);";
    html += "</script>";
    html += "</body></html>";

    request->send(200, "text/html", html);
  });

  // Handle AJAX request to get sensor data
  server.on("/getSensorData", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.readTemperature(); // Read temperature from DHT sensor
    float humidity = dht.readHumidity();       // Read humidity from DHT sensor

    // Create a JSON response
    String json = "{";
    json += "\"temperature\": " + String(temperature, 1) + ",";
    json += "\"humidity\": " + String(humidity, 1);
    json += "}";

    request->send(200, "application/json", json);
  });

  // Start server
  server.begin();
}

void loop() {
  // No additional code in the loop function
}
