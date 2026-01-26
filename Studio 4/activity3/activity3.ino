#include <avr/io.h>
#include "Arduino.h"

void setup() {
    cli();

    // 1. Set PB1 (Pin 9 on Arduino) as output
    DDRB |= (1 << PB1);

    // 2. Set Phase Correct PWM mode
    TCCR1A = ?;

    // Set Prescaler
    // This has to be set early for Timer1. 
    // By default, the Arduino Wiring language already 
    // sets up Timer1 in a different mode before this, which
    // makes our OCR1A = .. result in a wrong value otherwise  
    TCCR1B = ?;

    // 3. Set ICR1
    ICR1 = ?;

    // 4. Set OCR1A for duty cycle
    OCR1A = ?;

    TCNT1 = 0;

    // 5. Enable Non-Inverting Mode on OC1A (PB1)
    TCCR1A |= (1 << COM1A1);

    sei();
}

void loop() {

}
