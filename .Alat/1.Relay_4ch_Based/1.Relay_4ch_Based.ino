// Urutan pin relay harus sama dengan urutan pada relay
// Pin relay
const int relay1 = 26;    //pin1
const int relay2 = 27;    //pin2
const int relay3 = 12;    //pin3
const int relay4 = 13;    //pin4

void setup() {
  // Inisialisasi pin relay sebagai output
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  // Pastikan relay dimulai dalam keadaan mati
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  // Inisialisasi komunikasi serial
  Serial.begin(115200);
  Serial.println("ESP32 Relay Controller: Relay akan mati nyala otomatis setiap 5 detik.");

}

void loop() {
  // Nyala semua relay
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  Serial.println("Semua relay ON");
  delay(5000); // Tunggu 5 detik

  // Mati semua relay
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  Serial.println("Semua relay OFF");
  delay(5000); // Tunggu 5 detik
}
