#include <Key.h>
#include <Keypad.h>

#include "PCF8574.h"
#include <Wire.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' },
};

byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 7, 8, 12 };

Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char password[4] = { '1', '2', '3', '4' };
char currentPassword[4] = { '0', '0', '0', '0' };
byte currIndex = 0;

byte digitOnPins[4] = { 5, 6, 9, 10 };
byte segmentsPins[8] = { 11, 13, 100, A3, A2, A1, A0, 101 };

byte segmentDigitCodes[128];

PCF8574 expander(0x20);

void setup() {
  segmentDigitCodes['0'] = 0b10000001;
  segmentDigitCodes['1'] = 0b11001111;
  segmentDigitCodes['2'] = 0b10010010;
  segmentDigitCodes['3'] = 0b10000110;
  segmentDigitCodes['4'] = 0b11001100;
  segmentDigitCodes['5'] = 0b10100100;
  segmentDigitCodes['6'] = 0b10100000;
  segmentDigitCodes['7'] = 0b10001111;
  segmentDigitCodes['8'] = 0b10000000;
  segmentDigitCodes['9'] = 0b10000100;
  segmentDigitCodes['A'] = 0b10001000;
  segmentDigitCodes['B'] = 0b11100000;
  segmentDigitCodes['C'] = 0b10110001;
  segmentDigitCodes['D'] = 0b11000010;

  for (int i = 0; i < 4; i++) {
    pinMode(digitOnPins[i], OUTPUT);
  }

  for (int i = 0; i < 8; i++) {
    if (segmentsPins[i] < 100) {
      pinMode(segmentsPins[i], OUTPUT);
    }
  }
  expander.begin(0x20);
}

void loop() {
  char key = keypad.getKey();

    byte curr = segmentDigitCodes['1'];

    if(key) {
      curr = segmentDigitCodes[key];
    }
    
  

  byte arr[4] = { curr, segmentDigitCodes['2'], segmentDigitCodes['3'], segmentDigitCodes['4'] };

  WriteTo4x7Seg(arr);  
}

void WriteTo4x7Seg(byte chars[4]) {      
  for (int i = 0; i < 4; i++) {    
    for (int j = 0; j < 4; j++) {
      digitalWrite(digitOnPins[j], LOW);
    }     

    byte jChar = chars[i];
    for (int k = 0; k < 8; k++) {
      byte kBit = jChar & 1;
      jChar = jChar >> 1;
      if (segmentsPins[7 - k] >= 100) {
        expander.write(segmentsPins[7 - k] - 100, kBit == 0 ? LOW : HIGH);        
      }
      else {
        digitalWrite(segmentsPins[7 - k], kBit == 0 ? LOW : HIGH);
      }      
    }
    
    digitalWrite(digitOnPins[i], HIGH);
    delay(1);
  }
}












