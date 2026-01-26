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

void setup() {
  cli();

  TCCR0A = 0b10000001; // Set OCOM0A to 10 and WGM to 01

  TIMSK0 |= 0b10; // Enable Int for Output Compare Match

  OCR0A = 25;

  TCNT0 = 0;

  TCCR0B = 0b00000011; // Set clk source to clk/64

  //Set PORTD Pin 6 (Arduino Pin 6) as Output
  DDRD |= PIN6;

  sei();
}


// Warning: if you enable the timer interrupt, you must define
// the ISR, otherwise your Arduino might continually reset.
ISR(TIMER0_COMPA_vect)
{

}

void loop() {
  
}

