#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN7 (1 << 7)
#define PIN6 (1 << 6)
#define PIN5 (1 << 5)
#define PIN4 (1 << 4)
#define PIN3 (1 << 3)
#define PIN2 (1 << 2)
#define PIN1 (1 << 1)
#define PIN0 (1 << 0)

static volatile bool counting_up = false;

void setup() {
  cli();

  TCCR0A = 0b10000001; // Set OCOM0A to 10 and WGM to 01
  // set to 0b11000001 for complement
  TIMSK0 |= 0b10; // Enable Int for Output Compare Match

  OCR0A = 25;

  TCNT0 = 0;

  TCCR0B = 0b00000100; // Set clk source to clk/64
  //increase prescalar to slow down blinking light.

  //Set PORTD Pin 6 (Arduino Pin 6) as Output
  DDRD |= PIN6;

  sei();
}


// Warning: if you enable the timer interrupt, you must define
// the ISR, otherwise your Arduino might continually reset.
ISR(TIMER0_COMPA_vect) {
  if (OCR0A<=0) {
    counting_up = true;
  } else if (OCR0A>=25) {
    counting_up = false;
  }

  if (counting_up) {
    OCR0A++;
  } else {
    OCR0A--;
  }
  _delay_ms(10);
}

void loop() {
  
}

