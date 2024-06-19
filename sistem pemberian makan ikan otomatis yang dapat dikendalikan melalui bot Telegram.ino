#include <NTPClient.h>
#include <LCD_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

// Definisi Pin dan Konstanta
#define ONE_WIRE_BUS 12
#define WIFI_SSID "nama wifi"
#define WIFI_PASSWORD "passwordnya"
#define BOT_TOKEN "token telegram yang kamu buat di botfather"
#define BOT_MTBS 1000 // waktu antar pemindaian pesan
const int buzzer = 13;
const long utcOffsetInSeconds = 3600 * 7;

// Inisialisasi Objek dan Variabel
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Servo myservo;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
LCD_I2C lcd(0x27);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;
float tempC = 0;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
String waktu = "";

void setup() {
  // Inisialisasi Servo
  myservo.attach(2);
  myservo.write(0);
  
  // Inisialisasi Buzzer
  pinMode(buzzer, OUTPUT);

  // Inisialisasi LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Makanan Ikan  ");
  lcd.setCursor(0, 1);
  lcd.print("    Otomatis    ");
  delay(5000);

  // Inisialisasi Serial dan Sensor Suhu
  Serial.begin(115200);
  sensors.begin();

  // Koneksi ke WiFi
  Serial.print("Connecting to Wifi SSID ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setInsecure(); // Atur kepercayaan sertifikat

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  
  // Inisialisasi NTP Client
  timeClient.begin();
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/start") {
      String welcome = "Welcome to MAKIKOT, " + from_name + ".\n";
      welcome += "This is Chat Action example.\n\n";
      welcome += "MAKAN  : to feed the fish\n";
      welcome += "STATUS : to know the latest status & temperature\n";
      bot.sendMessage(chat_id, welcome);
    }

    if (text == "MAKAN") {
      waktu = "";
      myservo.write(70);
      bot.sendMessage(chat_id, "Memberi makan ikan...");
      delay(500);
      waktu += String(timeClient.getHours()) + ":" + String(timeClient.getMinutes());
      myservo.write(0);
      delay(500);
      myservo.write(70);
      delay(500);
      myservo.write(0);
      lcd.setCursor(0, 1);
      lcd.print("Terakhir ");
      lcd.setCursor(11, 1);
      lcd.print(waktu);
      lcd.print("    ");
    }

    if (text == "STATUS") {
      String ps = "Makan terakhir " + waktu + ". Suhu : " + String(tempC) + " Celcius.";
      bot.sendMessage(chat_id, ps);
    }
  }
}

void loop() {
  sensors.requestTemperatures(); // Mengambil data suhu
  timeClient.update();
  tempC = sensors.getTempCByIndex(0); // Mendapatkan suhu

  // Menampilkan Waktu dan Hari
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.print(",");
  lcd.setCursor(11, 0);
  if (timeClient.getHours() <= 9) lcd.print("0");
  lcd.print(timeClient.getHours());
  lcd.print(":");
  if (timeClient.getMinutes() <= 9) lcd.print("0");
  lcd.print(timeClient.getMinutes());

  // Cek pesan baru dari Telegram
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
}
