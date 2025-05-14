#include <PZEM004Tv30.h>

// Debugging Serial
#define DEBUG_SERIAL Serial

// Pin untuk PZEM 1
#define RX_PIN1 4
#define TX_PIN1 5

// Pin untuk PZEM 2
#define RX_PIN2 18
#define TX_PIN2 19

// Pin untuk PZEM 1
#define RX_PIN3 21
#define TX_PIN3 22

// Pin untuk PZEM 2
#define RX_PIN4 19
#define TX_PIN4 18

// HardwareSerial untuk masing-masing PZEM
HardwareSerial serialPZEM1(1); // UART1
HardwareSerial serialPZEM2(2); // UART2
HardwareSerial serialPZEM3(3); // UART3
HardwareSerial serialPZEM4(4); // UART4

// Objek PZEM untuk masing-masing modul
PZEM004Tv30 pzem1(serialPZEM1, RX_PIN1, TX_PIN1);
PZEM004Tv30 pzem2(serialPZEM2, RX_PIN2, TX_PIN2);
PZEM004Tv30 pzem3(serialPZEM3, RX_PIN3, TX_PIN3);
PZEM004Tv30 pzem4(serialPZEM4, RX_PIN4, TX_PIN4);

void setup() {
    // Inisialisasi Serial untuk debugging
    DEBUG_SERIAL.begin(115200);

    // Inisialisasi Serial untuk PZEM 1
    serialPZEM1.begin(9600, SERIAL_8N1, RX_PIN1, TX_PIN1);

    // Inisialisasi Serial untuk PZEM 2
    serialPZEM2.begin(9600, SERIAL_8N1, RX_PIN2, TX_PIN2);

        // Inisialisasi Serial untuk PZEM 2
    serialPZEM3.begin(9600, SERIAL_8N1, RX_PIN3, TX_PIN4);
        // Inisialisasi Serial untuk PZEM 2
    serialPZEM4.begin(9600, SERIAL_8N1, RX_PIN4, TX_PIN4);

    DEBUG_SERIAL.println("Monitoring Dua Modul PZEM-004T v3.0");
    DEBUG_SERIAL.println("------------------------------------");
}

void loop() {
    // Membaca data dari PZEM 1
    DEBUG_SERIAL.println("Data dari PZEM 1:");
    bacaPZEM(pzem1);

    // Membaca data dari PZEM 2
    DEBUG_SERIAL.println("Data dari PZEM 2:");
    bacaPZEM(pzem2);

    // Membaca data dari PZEM 1
    DEBUG_SERIAL.println("Data dari PZEM 3:");
    bacaPZEM(pzem3);

    // Membaca data dari PZEM 2
    DEBUG_SERIAL.println("Data dari PZEM 4:");
    bacaPZEM(pzem4);

    DEBUG_SERIAL.println("------------------------------------");

    // Tunggu 2 detik sebelum pembacaan berikutnya
    delay(2000);
}

void bacaPZEM(PZEM004Tv30 &pzem) {
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
    }
    DEBUG_SERIAL.println();
}
