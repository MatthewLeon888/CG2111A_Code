#include <avr/io.h>
#include "Arduino.h"

void setup() {
    // 1. Set PB1 (Pin 9 on Arduino) as output
    DDRB |= (1 << PB1);

    // 2. Set Phase Correct PWM mode
    TCCR1A = ?;
    TCCR1B = ?;

    // 3. Set ICR1
    ICR1 = ?;

    // 4. Set OCR1A for duty cycle
    OCR1A = ?;

    // 5. Enable Non-Inverting Mode on OC1A (PB1)
    TCCR1A |= (1 << COM1A1);
}

void loop() {

}
