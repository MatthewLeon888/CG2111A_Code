#include <avr/io.h>
#include "Arduino.h"

#define SERVO_SPEED 20 // Vaires the speed of flashing cycle of LED
static volatile bool counting_up = false; // Variable that toggles between modes
static volatile bool trigger = true;

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

// ISR(TIMER1_COMPA_vect) {
//   (trigger) ? PORTB |= 0b00001111 : PORTB &= ~0b00001111;
//   trigger = !trigger;
// }

// ISR(TIMER1_COMPA_vect) {
//   PORTC |= 0b00001111;
//   if (OCR1A <= 500) {
//     counting_up = true;
//   } else if (OCR1A >= 2800) {
//     counting_up = false;
//   }

//   if (counting_up) {
//     OCR1A += SERVO_SPEED;
//   } else {
//     OCR1A -= SERVO_SPEED;
//   }
// }

// ISR(TIMER1_COMPB_vect) {
//   PORTC &= ~0b00001111;
// }

void setup() {
  
  cli();

  // 1. Set PB1 (Pin 9 on Arduino) as output
  DDRB |= (1 << PB1);

  DDRC |= (1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0);

  // 2. Set Phase Correct PWM mode
  TCCR1A = 0b00000010;

  TIMSK1 = 0b00000110; // ADDED FOR PART 2

  // Set Prescaler
  // This has to be set early for Timer1. 
  // By default, the Arduino Wiring language already 
  // sets up Timer1 in a different mode before this, which
  // makes our OCR1A = .. result in a wrong value otherwise  
  TCCR1B = 0b00010010; //prescalar is 8

  // 3. Set ICR1
  ICR1 = 20000;

  // 4. Set OCR1A for duty cycle
  OCR1A = 1500;
  OCR1B = 0;
  
  TCNT1 = 0;

  // 5. Enable Non-Inverting Mode on OC1A (PB1)
  TCCR1A |= (1 << COM1A1);

  sei();
}

void loop() {

}
