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

static volatile int turn=0;

void flashGreen() {
  int count=1;
  int i;

  while(turn==0) {
    for(i=0; i<count && turn==0; i++) {
      PORTB |= 0b00010000; // Switch green LED at pin 12 on. Pin 12 is PB4
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11101111; //switch off
      _delay_ms(LED_DELAY);
    }
    _delay_ms(1000);
    count++;
  } 
}

void flashRed()
{
  int count=1;
  int i;

  while(turn==1) {
    for(i=0; i<count && turn==1; i++) {
      PORTB |= 0b00001000; // Switch red LED at pin 11 on
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11110111;
      _delay_ms(LED_DELAY);
    }
    _delay_ms(1000);
    count++;
  }
}

ISR(INT0_vect) {
  turn=1-turn;
}

void setup()
{
  // Set pins 11 and 12 to output. Pin 11 is PB3, pin 12 is PB4
  DDRB |= 0b00011000;
  // We need to set bits 0 and 1 in EICRA to 1
  EICRA |= 0b00000011;
  // Activate INT0
  EIMSK |= 0b00000001;
  // Ensure that interrupts are turned on.
  sei();

  // For Question 6,
  attachInterrupt(digitalPinToInterrupt(INT0), ISR, FALLING);

}


void loop()
{
  if(turn==0)
    flashGreen();
  else
    flashRed();
}
