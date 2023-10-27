#include <Wire.h>
#include <RTClib.h>  // Library RTC

RTC_DS3231 rtc;  // Inisialisasi objek RTC
const int GPIO_SDA = 21; // Pin GPIO untuk SDA
const int GPIO_SCL = 22; // Pin GPIO untuk SCL

void setup() {
  Serial.begin(115200);
  Wire.begin(GPIO_SDA, GPIO_SCL);
  rtc.begin();

  // Set waktu RTC jika diperlukan. Ini hanya perlu dilakukan sekali saat mengatur waktu awal.
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!rtc.isrunning()) {
    Serial.println("RTC tidak berjalan! Silakan set waktu RTC.");
  }
}

void loop() {
  DateTime now = rtc.now();  // Baca waktu dari RTC

  Serial.print("Tanggal dan Waktu: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);  // Tunggu 1 detik sebelum membaca waktu lagi
}
