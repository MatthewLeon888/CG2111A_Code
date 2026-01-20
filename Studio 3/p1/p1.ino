/*
 * w6s2p1.c
 *
 * Created: 2/2/2018 6:01:01 AM
 * Author : dcstanc
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DELAY 100

static volatile int turn=0;// The turn variable decides whose turn it is to go now.
static unsigned long _timerTicks = 0;// The _timerTicks variable maintains a count of how many 100us ticks have passed by.
// Our last time and current time variables
static unsigned long _lastTime = 0;
static unsigned long _currentTime = 0;


// The THRESHOLD value for debouncing
#define THRESHOLD   50  // We are using 100us ticks so this is equivalent to 5 ms.

// Our INT0 ISR just flips the turn variable
ISR(INT0_vect) {
  // Modify this ISR to do switch debouncing
  turn = 1 - turn;
}

// Uncomment and properly declare the ISR below for timer 2
// ISR(...) {
//   _timerTicks++;
// }

// Flash the green LED
void flashGreen() {
  int count=1;
  int i;

  while(turn==0) {
    for(i=0; i<count && turn==0; i++) {
      PORTB |= 0b00010000; // Switch green LED at pin 12 on. Pin 12 is PB4
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11101111;
      _delay_ms(LED_DELAY);
    }
    
    _delay_ms(1000);
    count++;
  } 
}

void flashRed() {
  int count=1;
  int i;

  while(turn==1) {
    for(i=0; i<count && turn==1; i++) {
      PORTB |= 0b00001000; // Switch green LED at pin 12 on. Pin 12 is PB4
      _delay_ms(LED_DELAY); // Delay 250ms
      PORTB &= 0b11110111;
      _delay_ms(LED_DELAY);
    }
    
    _delay_ms(1000);
    count++;
  }
}

// Timer set up function. 
void setupTimer() {
  // Set timer 2 to produce 100 microsecond (100us) ticks 
  // But do no start the timer here.
}

// Timer start function
void startTimer() {
  // Start timer 2 here.
}

void setup() {
  cli(); // Disable interrupts while setting up
  
  // Set up EICRA
  EICRA |= 0b00000011; // For falling edge on INT0, Bits 1 and 0 should be 10
  DDRB |= 0b00011000; // Set pins 11 and 12 to output. Pin 11 is PB3, pin 12 is PB4

  setupTimer(); // Set up the timer
  
  EIMSK |= 0b00000001; // Enable INT0
  
  startTimer(); // Start the timer

  sei(); // Enable interrupts
}

void loop() {
    if(turn==0)
      flashGreen();
    else
      flashRed();
}
