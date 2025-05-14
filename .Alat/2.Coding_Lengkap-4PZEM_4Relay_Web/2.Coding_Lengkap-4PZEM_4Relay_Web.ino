#include <WiFi.h>
#include <PZEM004Tv30.h>

// WiFi credentials
const char* ssid = "iphone";
const char* password = "12345678";

// Pin untuk PZEM
#define RX_PIN1 32
#define TX_PIN1 33
#define RX_PIN2 25
#define TX_PIN2 26
#define RX_PIN3 21
#define TX_PIN3 22
#define RX_PIN4 12
#define TX_PIN4 13

// Pin untuk relay
const int relay1 = 17;
const int relay2 = 16;
const int relay3 = 4;
const int relay4 = 15;

// HardwareSerial untuk masing-masing PZEM
HardwareSerial serialPZEM1(1);
HardwareSerial serialPZEM2(2);
HardwareSerial serialPZEM3(3);
HardwareSerial serialPZEM4(4);

// Objek PZEM untuk masing-masing modul
PZEM004Tv30 pzem1(serialPZEM1, RX_PIN1, TX_PIN1);
PZEM004Tv30 pzem2(serialPZEM2, RX_PIN2, TX_PIN2);
PZEM004Tv30 pzem3(serialPZEM3, RX_PIN3, TX_PIN3);
PZEM004Tv30 pzem4(serialPZEM4, RX_PIN4, TX_PIN4);

WiFiServer server(80);

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);

    // Connecting to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Start the web server
    server.begin();

    // Initialize PZEM
    serialPZEM1.begin(9600, SERIAL_8N1, RX_PIN1, TX_PIN1);
    serialPZEM2.begin(9600, SERIAL_8N1, RX_PIN2, TX_PIN2);
    serialPZEM3.begin(9600, SERIAL_8N1, RX_PIN3, TX_PIN3);
    serialPZEM4.begin(9600, SERIAL_8N1, RX_PIN4, TX_PIN4);

    // Initialize relay pins
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);
    digitalWrite(relay1, HIGH);  // Initial state OFF
    digitalWrite(relay2, HIGH);  // Initial state OFF
    digitalWrite(relay3, HIGH);  // Initial state OFF
    digitalWrite(relay4, HIGH);  // Initial state OFF

    Serial.println("Ready to handle relay and PZEM data.");
}

void loop() {
    // Handle incoming client connections
    WiFiClient client = server.available();
    if (client) {
        handleClient(client);
    }

    // Print IP address and free heap memory for debugging
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());

    // Read PZEM data
    Serial.println("Reading PZEM Data...");
    bacaPZEM(pzem1, 1);
    bacaPZEM(pzem2, 2);
    bacaPZEM(pzem3, 3);
    bacaPZEM(pzem4, 4);

    // Delay to prevent spamming of serial output
    delay(1000);
}

void handleClient(WiFiClient &client) {
    // Read the HTTP request from the client
    String request = client.readStringUntil('\r');
    client.flush();

    // Handle relay control based on the HTTP request
    if (request.indexOf("/relay1/on") != -1) {
        digitalWrite(relay1, LOW);  // Turn on relay1
    } else if (request.indexOf("/relay1/off") != -1) {
        digitalWrite(relay1, HIGH);  // Turn off relay1
    } else if (request.indexOf("/relay2/on") != -1) {
        digitalWrite(relay2, LOW);  // Turn on relay2
    } else if (request.indexOf("/relay2/off") != -1) {
        digitalWrite(relay2, HIGH);  // Turn off relay2
    } else if (request.indexOf("/relay3/on") != -1) {
        digitalWrite(relay3, LOW);  // Turn on relay3
    } else if (request.indexOf("/relay3/off") != -1) {
        digitalWrite(relay3, HIGH);  // Turn off relay3
    } else if (request.indexOf("/relay4/on") != -1) {
        digitalWrite(relay4, LOW);  // Turn on relay4
    } else if (request.indexOf("/relay4/off") != -1) {
        digitalWrite(relay4, HIGH);  // Turn off relay4
    }

    // Read data from PZEM modules
    String pzemData = readPzemData();

    // Send HTTP response with HTML, CSS, and PZEM Data
    String html = "<!DOCTYPE html>";
    html += "<html lang=\"en\">";
    html += "<head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "<title>Relay and PZEM Control</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }";
    html += ".container { background-color: #ffffff; border-radius: 8px; padding: 20px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); width: 90%; max-width: 800px; }";
    html += "h1 { text-align: center; color: #333; font-size: 2em; }";
    html += ".relay-buttons { display: flex; flex-wrap: wrap; justify-content: center; gap: 10px; margin-top: 20px; }";
    html += ".button { text-decoration: none; background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px; font-size: 1.1em; transition: background-color 0.3s; }";
    html += ".button:hover { background-color: #45a049; }";
    html += ".status { font-size: 1.2em; margin-top: 20px; text-align: center; color: #333; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    html += "th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }";
    html += "th { background-color: #4CAF50; color: white; }";
    html += "tr:hover { background-color: #f1f1f1; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<div class=\"container\">";
    html += "<h1>Relay and PZEM Control</h1>";

    html += "<div class=\"relay-buttons\">";
    html += "<a href=\"/relay1/on\" class=\"button\">Relay 1 ON</a>";
    html += "<a href=\"/relay1/off\" class=\"button\">Relay 1 OFF</a>";
    html += "<a href=\"/relay2/on\" class=\"button\">Relay 2 ON</a>";
    html += "<a href=\"/relay2/off\" class=\"button\">Relay 2 OFF</a>";
    html += "<a href=\"/relay3/on\" class=\"button\">Relay 3 ON</a>";
    html += "<a href=\"/relay3/off\" class=\"button\">Relay 3 OFF</a>";
    html += "<a href=\"/relay4/on\" class=\"button\">Relay 4 ON</a>";
    html += "<a href=\"/relay4/off\" class=\"button\">Relay 4 OFF</a>";
    html += "</div>";

    html += "<h2>PZEM Data</h2>";
    html += "<table>";
    html += "<tr><th>Voltage (V)</th><th>Current (A)</th><th>Power (W)</th><th>Energy (kWh)</th><th>Frequency (Hz)</th><th>Power Factor</th></tr>";
    html += pzemData; // Insert the PZEM data in the table
    html += "</table>";

    html += "</div>";
    html += "</body>";
    html += "</html>";

    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print(html);

    client.stop();
}

String readPzemData() {
    String data = "";

    // Read PZEM data for all 4 modules
    for (int i = 0; i < 4; i++) {
        PZEM004Tv30 *pzem;
        switch (i) {
            case 0: pzem = &pzem1; break;
            case 1: pzem = &pzem2; break;
            case 2: pzem = &pzem3; break;
            case 3: pzem = &pzem4; break;
        }

        // Get PZEM data
        float voltage = pzem->voltage();
        float current = pzem->current();
        float power = pzem->power();
        float energy = pzem->energy();
        float frequency = pzem->frequency();
        float pf = pzem->pf();

        // Check if the data is valid
        if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy) || isnan(frequency) || isnan(pf)) {
            data += "<tr><td colspan='6'>Failed to read PZEM data</td></tr>";
        } else {
            data += "<tr>";
            data += "<td>" + String(voltage, 2) + "</td>";
            data += "<td>" + String(current, 2) + "</td>";
            data += "<td>" + String(power, 2) + "</td>";
            data += "<td>" + String(energy, 3) + "</td>";
            data += "<td>" + String(frequency, 1) + "</td>";
            data += "<td>" + String(pf, 2) + "</td>";
            data += "</tr>";
        }
    }

    return data;
}



void bacaPZEM(PZEM004Tv30 &pzem, int number) {
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy) || isnan(frequency) || isnan(pf)) {
        Serial.println("Failed to read data from PZEM!");
    } else {
        Serial.print("PZEM ");
        Serial.print(number);
        Serial.print(" - Voltage: ");      Serial.print(voltage);      Serial.println(" V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println(" A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println(" W");
        Serial.print("Energy: ");       Serial.print(energy, 3);    Serial.println(" kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println(" Hz");
        Serial.print("Power Factor: "); Serial.println(pf);
    }
    Serial.println();
}
