/*A basic 8 channel receiver using an Arduino Pro Mini and the nRF24L01 module - code originally by Electronoobs.
 Code modified by Max Imagination to suit the Arduino-based Pistol-grip transmitter.
 Like, share and subscribe to ELECTRONOOBS & Max Imagination.
 http://www.youtube/c/electronoobs
 &
 https://www.youtube.com/max-imagination

 First we include the libraries. Download it from
   my webpage if you don't have the NRF24 library */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>      //To create PWM signals we need this lybrary

const uint64_t pipeIn = 0xE8E8F0F0E1LL; // Remember that this code is the same as in the transmitter
RF24 radio(9, 10); // CSN and CE pins

struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
  byte ch8;
};

Received_data received_data;

Servo channel_1;
Servo channel_2;
Servo channel_3;
Servo channel_4;

void reset_the_Data() {
  received_data.ch1 = 127;
  received_data.ch2 = 127;
  received_data.ch3 = 127;
  received_data.ch4 = 127;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;
  received_data.ch8 = 0;
}

void setup() {
  // Attach servos to pins for channels 1-4
  channel_1.attach(2);
  channel_2.attach(3);
  channel_3.attach(4);
  channel_4.attach(5);
  
  // Set AUX channels as digital outputs
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, OUTPUT);

  reset_the_Data();

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}

unsigned long lastRecvTime = 0;

void receive_the_data() {
  while (radio.available()) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis();
  }
}

void loop() {
  receive_the_data();

  unsigned long now = millis();
  if (now - lastRecvTime > 1000) {
    reset_the_Data();
  }

  int ch1_value = map(received_data.ch1, 0, 255, 1000, 2000);
  int ch2_value = map(received_data.ch2, 0, 255, 1000, 2000);
  int ch3_value = map(received_data.ch3, 0, 255, 1000, 2000);
  int ch4_value = map(received_data.ch4, 0, 255, 1000, 2000);

  channel_1.writeMicroseconds(ch1_value);
  channel_2.writeMicroseconds(ch2_value);
  channel_3.writeMicroseconds(ch3_value);
  channel_4.writeMicroseconds(ch4_value);

  // Set AUX channels as HIGH or LOW based on received data
  digitalWrite(6, received_data.ch5 == 1 ? HIGH : LOW);
  digitalWrite(7, received_data.ch6 == 1 ? HIGH : LOW);
  digitalWrite(8, received_data.ch7 == 1 ? HIGH : LOW);
  digitalWrite(A0, received_data.ch8 == 1 ? HIGH : LOW);
}
