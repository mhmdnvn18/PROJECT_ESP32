#include <ESP8266WiFi.h>
#include <PZEM004Tv30.h>

// Deklarasi SSID dan password WiFi
const char* ssid = "HOKYA2";
const char* password = "NOCONCIBAOTROSHIJOS";

// Objek WiFi Server
WiFiServer server(80);

// Objek sensor PZEM
PZEM004Tv30 pzem1(8, 0);  // GPIO2 (Tx) ke Rx PZEM; GPIO0 (Rx) ke Tx PZEM

// Pin untuk relay
const int relayPin = 2; // GPIO2

// Variabel untuk menyimpan data sensor
float voltage1, current1, power1, energy1, frequency1, pf1;

void setup() {
  Serial.begin(9600);
  delay(100);

  // Inisialisasi pin relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Relay awal dalam keadaan OFF

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Memulai server pada port 80
  server.begin();
  Serial.println("Server started");

  // Menampilkan IP awal
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Menampilkan IP ESP ke Serial Monitor
}

void loop() {
  // Membaca data dari sensor PZEM
  voltage1 = zeroIfNan(pzem1.voltage());
  current1 = zeroIfNan(pzem1.current());
  power1 = zeroIfNan(pzem1.power());
  energy1 = zeroIfNan(pzem1.energy());
  frequency1 = zeroIfNan(pzem1.frequency());
  pf1 = zeroIfNan(pzem1.pf());

  // Menampilkan data ke Serial Monitor
  Serial.printf("Voltage: %.2f V\n", voltage1);
  Serial.printf("Current: %.2f A\n", current1);
  Serial.printf("Power: %.2f W\n", power1);
  Serial.printf("Energy: %.2f kWh\n", energy1);
  Serial.printf("Frequency: %.2f Hz\n", frequency1);
  Serial.printf("Power Factor: %.2f\n", pf1);
  Serial.println("-------------------------------");

  // Menampilkan IP setiap 5 detik
  static unsigned long lastIPPrint = 0;
  if (millis() - lastIPPrint >= 5000) {
    lastIPPrint = millis();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // Menangani permintaan klien
  WiFiClient client = server.available();
  if (client) {
    handleClient(client);
  }

  delay(1000);
}

// Fungsi untuk mengatasi nilai NaN (Not a Number)
float zeroIfNan(float v) {
  return isnan(v) ? 0 : v;
}

// Fungsi untuk menangani permintaan klien
void handleClient(WiFiClient client) {
  String request = client.readStringUntil('\r');
  client.flush();

  // Mengontrol relay
  if (request.indexOf("/relay/on") != -1) {
    digitalWrite(relayPin, HIGH); // Menyalakan relay
  } else if (request.indexOf("/relay/off") != -1) {
    digitalWrite(relayPin, LOW); // Mematikan relay
  }

  // Membalas permintaan klien
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  // Halaman HTML
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1>Data Sensor PZEM</h1>");
  client.println("<p>Voltage: " + String(voltage1) + " V</p>");
  client.println("<p>Current: " + String(current1) + " A</p>");
  client.println("<p>Power: " + String(power1) + " W</p>");
  client.println("<p>Energy: " + String(energy1) + " kWh</p>");
  client.println("<p>Frequency: " + String(frequency1) + " Hz</p>");
  client.println("<p>Power Factor: " + String(pf1) + "</p>");
  client.println("<h2>Relay Control</h2>");
  client.println("<p><a href=\"/relay/on\">Turn Relay ON</a></p>");
  client.println("<p><a href=\"/relay/off\">Turn Relay OFF</a></p>");
  client.println("</html>");
  delay(1);
}
