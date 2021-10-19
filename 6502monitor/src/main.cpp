#include <Arduino.h>
#include "decode.h"

#define NUM_DATA 8;
#define NUM_ADDR 16

// Using ben eater's pinout because it makes sense
constexpr char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
constexpr char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
constexpr char CLOCK = 2;
constexpr char RW = 3;


void setup()
{
    // Set up data pins
    for (int i = 0; i < NUM_DATA; i++) {
        pinMode(DATA[i], INPUT);
    }
    
    // Set up address pins
    for (int i = 0; i < NUM_ADDR; i++) {
        pinMode(ADDR[i], INPUT);
    }

    // Set up misc pins
    pinMode(CLOCK, INPUT);
    pinMode(RW, INPUT);

    attachInterrupt(digitalPinToInterrupt(CLOCK), tick, RISING);

    Serial.begin(57600);
}

void loop(){}

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
    for (int n = 0; n < NUM_DATA; n++) {
        int bit = digitalRead(DATA[n]) ? 1 : 0;
        Serial.print(bit);
        data = (data << 1) + bit;
    }

    sprintf(out, "   %04x  %c %02x", addr, digitalRead(RW) ? 'r' : 'W', data);
    Serial.println(out);
}