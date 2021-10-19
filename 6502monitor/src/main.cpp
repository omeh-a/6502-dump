#include <Arduino.h>
#include <stdio.h>
// #include "decode.h"

constexpr int NUM_DATA = 8;
constexpr int NUM_ADDR = 16;

// Using ben eater's pinout because it makes sense
constexpr char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
constexpr char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
constexpr char CLOCK = 2;
constexpr char RW = 3;

void tick();

void setup()
{
    Serial.begin(57600);

    // Set up data pins
    Serial.print("\nData pins: ");
    for (int i = 0; i < NUM_DATA; i++) {
        pinMode(DATA[i], INPUT);
        Serial.print((int)DATA[i]);
        Serial.print(" ");
    }
    
    // Set up address pins
    Serial.print("\nAddress pins: ");
    for (int i = 0; i < NUM_ADDR; i++) {
        pinMode(ADDR[i], INPUT);
        Serial.print((int)ADDR[i]);
        Serial.print(" ");
    }

    // Set up misc pins
    pinMode(CLOCK, INPUT);
    Serial.print("\nClock pin: ");
    Serial.print((int)CLOCK);

    pinMode(RW, INPUT);
    Serial.print("\nRead/write mode pin: ");
    Serial.print((int)RW);

    attachInterrupt(digitalPinToInterrupt(CLOCK), tick, RISING);

    Serial.print("\nREADY");
}

void loop(){
}

void tick() {
    char out[NUM_ADDR];
    uint16_t addr = 0;
    uint8_t data = 0;

    Serial.print("ADDR::");

    // Collect address
    for (int n = 0; n < NUM_ADDR; n++) {
        int bit = digitalRead(ADDR[n]) ? 1 : 0;
        Serial.print(bit);
        addr = (addr << 1) + bit;
    }
    Serial.print("   DATA::");

    // Collect data
    for (int i = 0; i < 8; i +=1) {
        int bit = digitalRead(DATA[i]) ? 1 : 0;
        Serial.print(bit);
        data = (data << 1) + bit;
    }

    sprintf(out, "   %04x  %c %02x", addr, digitalRead(RW) ? 'r' : 'W', data);
    Serial.println(out);
}