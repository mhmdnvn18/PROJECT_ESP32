#include <PZEM004Tv30.h>

// Gunakan Serial untuk debugging
#define DEBUG_SERIAL Serial

// Pilih pin RX dan TX yang akan digunakan
#define RX_PIN 4
#define TX_PIN 5

// Buat objek HardwareSerial
HardwareSerial customSerial(1); // UART1 pada ESP32
PZEM004Tv30 pzem(customSerial, RX_PIN, TX_PIN);

void setup() {
    // Inisialisasi Serial untuk debugging
    DEBUG_SERIAL.begin(115200);

    // Inisialisasi komunikasi dengan PZEM melalui UART1
    customSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

    DEBUG_SERIAL.println("PZEM-004T v3.0 Monitoring");
    DEBUG_SERIAL.println("-------------------------");
}

void loop() {
    // Baca data dari modul PZEM
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();

    // Periksa apakah data valid
    if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy) || isnan(frequency) || isnan(pf)) {
        DEBUG_SERIAL.println("Gagal membaca data dari PZEM!");
    } else {
        // Tampilkan data ke Serial Monitor
        DEBUG_SERIAL.print("Voltage: ");      DEBUG_SERIAL.print(voltage);      DEBUG_SERIAL.println(" V");
        DEBUG_SERIAL.print("Current: ");      DEBUG_SERIAL.print(current);      DEBUG_SERIAL.println(" A");
        DEBUG_SERIAL.print("Power: ");        DEBUG_SERIAL.print(power);        DEBUG_SERIAL.println(" W");
        DEBUG_SERIAL.print("Energy: ");       DEBUG_SERIAL.print(energy, 3);    DEBUG_SERIAL.println(" kWh");
        DEBUG_SERIAL.print("Frequency: ");    DEBUG_SERIAL.print(frequency, 1); DEBUG_SERIAL.println(" Hz");
        DEBUG_SERIAL.print("Power Factor: "); DEBUG_SERIAL.println(pf);
        DEBUG_SERIAL.println("-------------------------");
    }

    // Tunggu 2 detik sebelum pembacaan berikutnya
    delay(2000);
}
