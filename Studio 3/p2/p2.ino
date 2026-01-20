#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void setup() {
  // Clear interrupts for safety while setting up
  cli();

  DDRD |= (1 << DDD2); // Only Arduino Pin 2 as output (PD2)
  EICRA |= 0b00000011; // For falling edge on INT0, Bits 1 and 0 should be 10

  setupTimer(); // Set up timer 1

  startTimer(); // Start timer 1

  sei(); // Ensure that interrupts are enabled
}

// Timer set up function. 
void setupTimer() {
  // Set timer 1 to produce 20ms (1000000us) ticks 
  // But do not start the timer here.
}

// Timer start function
void startTimer() {
  // Start timer 1 here.
}

void loop() {
   // Everything is in the ISRs
}
