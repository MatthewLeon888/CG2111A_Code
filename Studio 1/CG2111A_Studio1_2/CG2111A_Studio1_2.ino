#include "Arduino.h"

// Swtiches need D4, D5, D6, D7
// Switches need Port D Pin 4, 5, 6, 7
// Ardunio Pins are 4, 5, 6, 7

// LEDs need D10, D11, D12, D13
// LEDs need Port B Pin 2, 3, 4, 5
// Ardunio Pins are 10, 11, 12, 13

#define PIN4 (1 << 4) // 0b00010000
#define PIN5 (1 << 5) // 0b00100000
#define PIN6 (1 << 6) // 0b01000000
#define PIN7 (1 << 7) // 0b10000000

#define PIN10 (1 << 2) // 0b00000100
#define PIN11 (1 << 3) // 0b00001000
#define PIN12 (1 << 4) // 0b00010000
#define PIN13 (1 << 5) // 0b00100000

void setup() {
  // setting PIN5 as output and PIN2 as input
  // PIN5 = 0b00100000
  // PIN2 = 0b00000100
  //~PIN2 = 0b11111011
  //DDRB = ((PIN5) & ~(PIN2));
  // DDRB = 0b00100000

  // setting PIN4, PIN5, PIN6, PIN7 as input
  DDRD = 0b00000000;

  // setting PIN10, PIN11, PIN12, PIN13 as output
  //DDRB |= (PINB2 | PINB3 | PINB4 | PINB5);
  DDRB |= 0b00111100;
  // DDRB = 0b00111100
}

void loop() {
  // PIND referes to all the pin outputs of Port D
  // example 1, if Arduino Pin 2 is ON, Port D Pin 2 will be ON, then PIND = 0b00000100
  // example 2, if Arduino Pin 7 is ON, Port D Pin 7 will be ON, then PIND = 0b10000000

  // RED LED control
  if(PIND & PIN4) {
    PORTB |= PIN10; // turn on red led
  }
  else {
    PORTB &= ~PIN10; // turn off red led
  }

  // GREEN LED control
  if(PIND & PIN5) {
    PORTB |= PIN11; // turn on green led
  }
  else {
    PORTB &= ~PIN11; // turn off green led
  }
  
  // OTHER RED LED control
  if(PIND & PIN6) {
    PORTB |= PIN12; // turn on other red led
  }
  else {
    PORTB &= ~PIN12; // turn off other red led
  }

  // OTHER GREEN LED control
  if(PIND & PIN7) {
    PORTB |= PIN13; // turn on other green led
  }
  else {
    PORTB &= ~PIN13; // turn off other green led
  }
}

