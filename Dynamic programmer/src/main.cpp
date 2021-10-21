#include <Arduino.h>
#include <stdio.h>

#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13
#define FULL_WIPE 1
#define SEMI_WIPE 0

void purgeBuffer(bool fullWipe);
char charFormat(char c);

constexpr size_t EEPROM_SIZE = 262144;
constexpr char ASCII_A_OFFSET = 55;
constexpr char ASCII_ZERO_OFFSET = 48;

uint8_t *buffer;
unsigned int used_bytes = 0;

void setup() {
    pinMode(SHIFT_DATA, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_LATCH, OUTPUT);
    pinMode(WRITE_EN, OUTPUT);

    Serial.begin(57600);
    Serial.println((int)sizeof(size_t));
    buffer = (uint8_t *) calloc(EEPROM_SIZE, sizeof(uint8_t));
    purgeBuffer(FULL_WIPE);
    Serial.println("READY\nWaiting for data...");
}

void loop() {
    
}

boolean write() {
    digitalWrite(WRITE_EN, HIGH);

}

void serialEvent() {
    Serial.print("Used ");
    Serial.print(used_bytes);
    Serial.print(" bytes out of  ");
    Serial.print(EEPROM_SIZE);
    while (Serial.available() && used_bytes < EEPROM_SIZE) {
        buffer[used_bytes] = (char)Serial.read();
        Serial.print(buffer[used_bytes]);
        Serial.print(", ");
        used_bytes++;
    }
}


// Selectively clears data buffer. fullWipe flag specifies
// if all of the buffer should be cleared (for initialisation)
// or just the number of bytes which were last used in a read.
void purgeBuffer(bool fullWipe) {
    if (fullWipe) {
        for (unsigned long i = 0; i < EEPROM_SIZE; i++) {
            buffer[i] = 0;
        }
    } else {
        if (used_bytes < 0) {
            Serial.println("Failed to purge buffer - no bytes used.");
        }
        for (unsigned long i = 0; i < used_bytes - 1 && i < EEPROM_SIZE; i++) {
            buffer[i] = 0;
        }
    }
    used_bytes = 0;
    Serial.println("Successfully purged buffer.");
}

char charFormat(char c) {
    // check for bad input
    if (c > 16) {
        return '!';
    }

    if (c >= 10) {
        return c + ASCII_A_OFFSET;
    }
    return c + ASCII_ZERO_OFFSET;
}