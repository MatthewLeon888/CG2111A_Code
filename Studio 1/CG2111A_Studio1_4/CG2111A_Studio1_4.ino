#include "Arduino.h"

// GOAL: Counts from 0 to 9 and loops
// "X" needs a, b, c, d, e, f, g => D, C, B, A
// "0" needs H, H, H, H, H, H, L => L, L, L, L
// "1" needs L, H, H, L, L, L, L => L, L, L, H
// "2" needs H, H, L, H, H, L, H => L, L, H, L
// "3" needs H, H, H, H, L, L, H => L, L, H, H
// "4" needs L, H, H, L, L, H, H => L, H, L, L
// "5" needs H, L, H, H, L, H, H => L, H, L, H
// "6" needs L, L, H, H, H, H, H => L, H, H, L
// "7" needs H, H, H, L, L, L, L => L, H, H, H
// "8" needs H, H, H, H, H, H, H => H, L, L, L
// "9" needs H, H, H, L, L, H, H => H, L, L, H

// Port D Pin 4, 5, 6, 7
// Ardunio Pins are 4, 5, 6, 7
#define PIN4A (1 << 4) // 0b00010000 (INPUT A of 7447 decoder)
#define PIN5B (1 << 5) // 0b00100000 (INPUT B of 7447 decoder)
#define PIN6C (1 << 6) // 0b01000000 (INPUT C of 7447 decoder)
#define PIN7D (1 << 7) // 0b10000000 (INPUT D of 7447 decoder)

void setup() {
  // setting PIN4A, PIN5B, PIN6C, PIN7D as output
  DDRD |= (PIN4A | PIN5B | PIN6C | PIN7D);
  PORTD = 0b00000000;
}

void loop() {
  // Display "0"
  // ~(PIN4A | PIN5B | PIN6C | PIN7D) = ~(0b11110000) = 0b00001111
  PORTD &= ~(PIN4A | PIN5B | PIN6C | PIN7D);
  // PORTD = 0b00000000
  delay(1000);

  // Display "1"
  PORTD |= PIN4A;
  // PORTD = 0b00010000
  delay(1000);

  // Display "2"
  //~PIN4A = 0b11101111
  PORTD &= ~PIN4A;
  // PORTD = 0b00000000
  PORTD |= PIN5B;
  // PORTD = 0b00100000
  delay(1000);

  // Display "3"
  PORTD |= PIN4A;
  // PORTD = 0b00110000
  delay(1000);

  // Display "4"
  //~(PIN4A | PIN5B) = ~(0b00110000) = 0b11001111
  PORTD &= ~(PIN4A | PIN5B);
  // PORTD = 0b00000000
  PORTD |= PIN6C;
  // PORTD = 0b01000000
  delay(1000);

  // Display "5"
  PORTD |= PIN4A;
  // PORTD = 0b01010000
  delay(1000);

  // Display "6"
  //~PIN4A = 0b11101111
  PORTD &= ~PIN4A;
  // PORTD = 0b01000000
  PORTD |= PIN5B;
  // PORTD = 0b01100000
  delay(1000);

  // Display "7"
  PORTD |= PIN4A;
  delay(1000);

  // Display "8"
  PORTD &= ~(PIN4A | PIN5B | PIN6C);
  PORTD |= PIN7D;
  delay(1000);

  // Display "9"
  PORTD |= PIN4A;
  delay(1000);

  // Display "10" ????
  PORTD &= ~PIN4A;
  PORTD |= PIN5B;
  delay(1000);

  PORTD |= PIN4A;
  delay(1000);

  PORTD &= ~(PIN4A | PIN5B);
  PORTD |= PIN6C;
  delay(1000);

  PORTD |= PIN4A;
  delay(1000);

  PORTD &= ~PIN4A;
  PORTD |= PIN5B;
  delay(1000);

  PORTD |= PIN4A;
  delay(1000);
}


