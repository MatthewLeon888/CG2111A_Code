#include <avr/io.h>
#include "Arduino.h"

static volatile bool flag = false;

ISR(TIMER1_COMPA_vect) {
  // When triggered, set servo pin to HIGH
  PORTD |= PB1;
  flag = true;
}

ISR(TIMER1_COMPB_vect) {
  if (flag) {
    PORTD &= ~PB1;
    flag = false;
  }
}

void setup() {
  cli();

  // 1. Set PB1 (Pin 9 on Arduino) as output
  DDRB |= (1 << PB1);

  // For falling edge on INT0, Bits 1 and 0 should be 10
  EICRA |= 0b00000011; // ADDED FOR PART 2

  // 2. Set Phase Correct PWM mode
  TCCR1A = 0b10100011;

  TIMSK1 = 0b110; // ADDED FOR PART 2

  // Set Prescaler
  // This has to be set early for Timer1. 
  // By default, the Arduino Wiring language already 
  // sets up Timer1 in a different mode before this, which
  // makes our OCR1A = .. result in a wrong value otherwise  
  TCCR1B = 0b00010010;

  // 3. Set ICR1
  ICR1 = 20000;

  // 4. Set OCR1A for duty cycle
  OCR1A = 40000;
  OCR1B = 3000;

  TCNT1 = 0;

  // 5. Enable Non-Inverting Mode on OC1A (PB1)
  TCCR1A |= (1 << COM1A1);

  sei();
}

void loop() {

}
