#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static volatile bool A_triggered = false;

//int count = 0;
// static volatile unsigned long _timerTicks = 0;// The _timerTicks variable maintains a count of how many 100us ticks have passed by.
// // Our last time and current time variables
// static volatile unsigned long _lastTime = 0;
// static volatile unsigned long _currentTime = 0;

// Timer set up function. 
void setupTimer() {
  // Set timer 1 to produce 20ms (1000000us) ticks 
  // But do not start the timer here.
  // Set Initial Timer value
  
  // Set CTC mode and make toggle PD6/OC0A pin on compare match
  TCCR1A = 0b00000010;
  TCCR1B = 0b00000000;
  // Enable interrupts.
  TIMSK1 = 0b110;
  TCNT1 = 0;

  // Place TOP timer value to Output compare register
  OCR1A = 39999;
  OCR1B = 2999;
}

// Timer start function
void startTimer() {
  // Start timer 1 here.
  //Set prescaler 8 and start timer
  TCCR1B = 0b00001010;
  // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
  // When triggered, set servo pin to HIGH
  PORTD |= 0b00000010;
  A_triggered = true;
}

ISR(TIMER1_COMPB_vect) {
  if (A_triggered) {
    PORTD &= 0b11111101;
    A_triggered = false;
  }
}

void setup() {
  // Clear interrupts for safety while setting up
  cli();

  // Only Arduino Pin 2 as output (PD2)
  DDRD |= (1 << DDD2);

  // Set up EICRA
  // For falling edge on INT0, Bits 1 and 0 should be 10
  EICRA |= 0b00000011;
  setupTimer(); // Set up timer 1
  startTimer(); // Start timer 1

  // Ensure that interrupts are enabled
  sei();
}

void loop() {
   // Everything is in the ISRs
}
