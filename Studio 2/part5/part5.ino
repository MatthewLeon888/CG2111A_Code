/*
 * w5s1p3
 *
 * Created: 2/2/2018 6:01:01 AM
 * Author : dcstanc
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DELAY  100

static volatile int turn = 0;

void flashGreen() {
  int count = 1;
  int i;

  while(turn == 0) {
    for(i = 0; i < count && turn == 0; i++) {
      PORTB |= 0b00010000;  // Switch green LED at pin 12 ON. Pin 12 is PB4
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11101111;  // Switch OFF
      _delay_ms(LED_DELAY);
    }
    count++;
    for (int i=0; i<5; i++) {
      if (turn==0) return;
      delay(200);
    }
  } 
}

void flashRed()
{
  int count = 1;
  int i;

  while(turn == 1) {
    for(i = 0; i < count && turn==1; i++) {
      PORTB |= 0b00001000;  // Switch red LED at pin 11 on
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11110111;  // Switch OFF
      _delay_ms(LED_DELAY);
    }
    count++;
    for (int i=0; i<5; i++) {
      if (turn==0) return;
      delay(200);
    }
  }
}

ISR(INT0_vect) {
  turn = 1 - turn;
}

void setup() {
  DDRB  |= 0b00011000; // Set pins 11 and 12 to output. Pin 11 is PB3, pin 12 is PB4
  EICRA |= 0b00000010; // We need to set bits 0 and 1 in EICRA to 1 (activate on FALLING)
  EIMSK |= 0b00000001; // Activate INT0_vect
  sei();               // Ensure that interrupts are turned on.

  // attachInterrupt(digitalPinToInterrupt(pin), , FALLING);
}

void loop()
{
  if(turn == 0) flashGreen();
  else flashRed();
}
