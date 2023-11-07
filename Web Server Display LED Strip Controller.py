#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebSrv.h>

const char* ssid = "nama_wifi";
const char* password = "password_wifi";

// Pinout untuk LED strip
const int pinLedG = 25;  // Pin untuk sinyal Green
const int pinLedR = 26;  // Pin untuk sinyal Red
const int pinLedB = 27;  // Pin untuk sinyal Blue

AsyncWebServer server(80);

bool isLedOn = false;
String currentColor = "#000000";
int effectMode = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pinLedG, OUTPUT);
  pinMode(pinLedR, OUTPUT);
  pinMode(pinLedB, OUTPUT);

  // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Menampilkan informasi IP
  Serial.println(WiFi.localIP());

  // Inisialisasi server web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getIndexPage());
  });

  server.on("/setcolor", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("color", true)) {
      String colorValue = request->getParam("color", true)->value();
      setColor(colorValue);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Invalid request");
    }
  });

  server.on("/toggleled", HTTP_GET, [](AsyncWebServerRequest *request){
    toggleLed();
    request->send(200, "text/plain", "OK");
  });

  server.on("/seteffect", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("effect", true)) {
      int effectValue = request->getParam("effect", true)->value().toInt();
      setEffect(effectValue);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Invalid request");
    }
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/css", getStyle());
  });

  server.begin();
}

void loop() {
  if (isLedOn) {
    switch (effectMode) {
      case 1:  // Efek mengalir
        flowEffect();
        break;
      case 2:  // Efek gelombang
        waveEffect();
        break;
      // Tambahkan efek lain sesuai kebutuhan
      default:
        break;
    }
  }
}

String getIndexPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link rel='stylesheet' href='/style.css'>";
  html += "</head><body>";
  html += "<h1>Control LED Strip</h1>";
  html += "<div class='container'>";
  html += "<input type='color' id='colorPicker' value='" + currentColor + "'>";
  html += "<button class='toggleBtn' onclick='toggleLed()'>" + (isLedOn ? String("Turn Off") : String("Turn On")) + "</button>";
  html += "<select id='effectSelector' onchange='setEffect()'>";
  html += "<option value='0'>No Effect</option>";
  html += "<option value='1'>Flow Effect</option>";
  html += "<option value='2'>Wave Effect</option>";
  // Tambahkan opsi efek lain sesuai kebutuhan
  html += "</select>";
  html += "</div>";
  html += "<script src='https://code.jquery.com/jquery-3.6.0.min.js'></script>";
  html += "<script>";
  html += "function toggleLed() {";
  html += "$.get('/toggleled');";
  html += "}";
  html += "function setEffect() {";
  html += "var effectValue = $('#effectSelector').val();";
  html += "$.post('/seteffect', { effect: effectValue });";
  html += "}";
  html += "$('#colorPicker').on('change', function() {";
  html += "$.post('/setcolor', { color: $(this).val() });";
  html += "});</script>";
  html += "</body></html>";
  return html;
}

void setColor(String color) {
  currentColor = color;
  int r = (int)strtol(color.substring(1, 3).c_str(), NULL, 16);
  int g = (int)strtol(color.substring(3, 5).c_str(), NULL, 16);
  int b = (int)strtol(color.substring(5, 7).c_str(), NULL, 16);

  analogWrite(pinLedG, g);
  analogWrite(pinLedR, r);
  analogWrite(pinLedB, b);
}

void toggleLed() {
  isLedOn = !isLedOn;
  if (!isLedOn) {
    analogWrite(pinLedG, 0);
    analogWrite(pinLedR, 0);
    analogWrite(pinLedB, 0);
  }
}

void setEffect(int effect) {
  effectMode = effect;
}

void flowEffect() {
  static int flowPosition = 0;
  static int flowSpeed = 5;

  flowPosition += flowSpeed;
  if (flowPosition >= 255 || flowPosition <= 0) {
    flowSpeed *= -1;
  }

  analogWrite(pinLedG, flowPosition);
  analogWrite(pinLedR, flowPosition);
  analogWrite(pinLedB, flowPosition);
}

void waveEffect() {
  static int wavePosition = 0;
  static int waveSpeed = 5;

  wavePosition += waveSpeed;
  if (wavePosition >= 768 || wavePosition <= 0) {
    waveSpeed *= -1;
  }

  if (wavePosition < 256) {
    analogWrite(pinLedG, wavePosition);
    analogWrite(pinLedR, 255 - wavePosition);
    analogWrite(pinLedB, 0);
  } else if (wavePosition < 512) {
    analogWrite(pinLedG, 255 - (wavePosition - 256));
    analogWrite(pinLedR, 0);
    analogWrite(pinLedB, wavePosition - 256);
  } else {
    analogWrite(pinLedG, 0);
    analogWrite(pinLedR, wavePosition - 512);
    analogWrite(pinLedB, 255 - (wavePosition - 512));
  }
}

String getStyle() {
  String css = "body { font-family: Arial, sans-serif; text-align: center; }";
  css += "h1 { margin-top: 30px; }";
  css += ".container { display: flex; flex-direction: column; align-items: center; margin-top: 20px; }";
  css += "#colorPicker { width: 200px; height: 40px; margin-bottom: 20px; }";
  css += ".toggleBtn { padding: 10px 20px; font-size: 16px; background-color: #4CAF50; color: white; border: none; cursor: pointer; }";
  css += "#effectSelector { width: 200px; height: 40px; }";
  return css;
}
