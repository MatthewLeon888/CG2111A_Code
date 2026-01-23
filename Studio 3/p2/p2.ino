#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static unsigned long _timerTicks = 0;

// Timer set up function. 
void setupTimer() {
  // Set timer 1 to produce 20ms (1000000us) ticks 
  // But do not start the timer here.
  
  // Set CTC mode and make toggle PD6/OC0A pin on compare match
  TCCR1A = 0b00000000;
  // Enable interrupts.
  TIMSK1 |= 0b010;
  // Set Initial Timer value
  TCNT1 = 0;
  // Place TOP timer value to Output compare register
  OCR1A = 39999;
}

// Timer start function
void startTimer() {
  // Start timer 1 here.
  //Set prescaler 8 and start timer
  TCCR1B = 0b00001010;
}

ISR(TIMER1_COMPA_vect) {
  // When triggered, set servo pin to HIGH for 1.5ms
  _timerTicks++;
  PORTD |= 0b00000010;
  _delay_ms(1.5);
  PORTD &= 0b11111101;
}

void setup() {
  // Clear interrupts for safety while setting up
  cli();

  // Only Arduino Pin 2 as output (PD2)
  DDRD |= (1 << DDD2);

  // Set up EICRA
  // For falling edge on INT0, Bits 1 and 0 should be 10
  EICRA |= 0b00000011;

  // Set up timer 1
  setupTimer();

  // Start timer 1
  startTimer();

  // Ensure that interrupts are enabled
  sei();
}

void loop() {
   // Everything is in the ISRs
}