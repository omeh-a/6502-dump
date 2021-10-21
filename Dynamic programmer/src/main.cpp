#include <Arduino.h>
#include <stdio.h>

#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13
#define WRITE_BUTTON 19
#define FULL_WIPE 1
#define SEMI_WIPE 0
#define BUTTON_THRESHOLD 1000

struct ListNode {
    ListNode *next;
    uint8_t value;
};

void purgeBuffer(bool fullWipe);
boolean write();

constexpr unsigned long EEPROM_SIZE = 262144;
constexpr char ASCII_A_OFFSET = 55;
constexpr char ASCII_ZERO_OFFSET = 48;

ListNode *buffer;
ListNode *current;
unsigned int used_bytes = 0;
unsigned long lastButtonPress = 0;

void setup() {
    pinMode(SHIFT_DATA, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_LATCH, OUTPUT);
    pinMode(WRITE_EN, OUTPUT);
    pinMode(WRITE_BUTTON, INPUT);

    Serial.begin(57600);

    buffer = (ListNode *) malloc(sizeof(ListNode));
    buffer->value = 0;
    buffer->next = NULL;
    current = buffer;

    ListNode * curr = buffer;
    // Build linked list
    for (unsigned long i = 0; i < EEPROM_SIZE; i++) {
        curr->next = (ListNode *) malloc(sizeof(ListNode));
        curr -> value = 0;
        curr = curr->next;
        // Serial.println(i);
    }


    // purgeBuffer(FULL_WIPE);
    Serial.println("READY\nWaiting for data...");
}

// Monitors write button
void loop() {
    int bState = analogRead(WRITE_BUTTON);
    
    if (bState > BUTTON_THRESHOLD) {
        // 3 second cooldown after pressing button
        if (millis() -  lastButtonPress > 3000) {
            write();

            // timestamp this button press for cooldown
            lastButtonPress = millis();
        }
    }
    

}

boolean write() {
    // digitalWrite(WRITE_EN, HIGH);
    ListNode * curr = buffer;
    while (curr->value != 0 && curr != NULL) {
        char str[4];
        sprintf(str,"0x%X", curr->value);
        Serial.print(str);
        Serial.print(", ");
        curr = curr->next;
    }
    return false;
}

void serialEvent() {
    while (Serial.available() && current != NULL) {
        current->value = (char)Serial.read();
        Serial.print(current->value);
        Serial.print(", ");
        current = current->next;
        used_bytes++;
    }
}


// Selectively clears data buffer. fullWipe flag specifies
// if all of the buffer should be cleared (for initialisation)
// or just the number of bytes which were last used in a read.
// void purgeBuffer(bool fullWipe) {
//     if (fullWipe) {
//         for (unsigned long i = 0; i < EEPROM_SIZE; i++) {
//             buffer[i] = 0;
//         }
//     } else {
//         if (used_bytes < 0) {
//             Serial.println("Failed to purge buffer - no bytes used.");
//         }
//         for (unsigned long i = 0; i < used_bytes - 1 && i < EEPROM_SIZE; i++) {
//             buffer[i] = 0;
//         }
//     }
//     used_bytes = 0;
//     Serial.println("Successfully purged buffer.");
// }
