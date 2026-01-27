#include <avr/io.h>
#include "Arduino.h"
#include <avr/interrupt.h>


#define SERVO_SPEED 20 // Vaires the speed of flashing cycle of LED
static volatile bool counting_up = false; // Variable that toggles between modes

ISR(TIMER1_COMPA_vect) {
  if (OCR1A <= 500) {
    counting_up = true;
  } else if (OCR1A >= 2800) {
    counting_up = false;
  }

  if (counting_up) {
    OCR1A += SERVO_SPEED;
  } else {
    OCR1A -= SERVO_SPEED;
  }
}


void setup() {
  
  cli();

  // 1. Set PB1 (Pin 9 on Arduino) as output
  DDRB |= (1 << PB1);

  // For falling edge on INT0, Bits 1 and 0 should be 10
  EICRA |= 0b00000011; // ADDED FOR PART 2

  // 2. Set Phase Correct PWM mode
  TCCR1A = 0b10000010;

  TIMSK1 = 0b10; // ADDED FOR PART 2

  // Set Prescaler
  // This has to be set early for Timer1. 
  // By default, the Arduino Wiring language already 
  // sets up Timer1 in a different mode before this, which
  // makes our OCR1A = .. result in a wrong value otherwise  
  TCCR1B = 0b00010010; //prescalar is 8

  // 3. Set ICR1
  ICR1 = 20000;

  // 4. Set OCR1A for duty cycle
  OCR1A = 1000;

  TCNT1 = 0;

  // 5. Enable Non-Inverting Mode on OC1A (PB1)
  TCCR1A |= (1 << COM1A1);

  sei();
}

void loop() {

}
