#include "Arduino.h"
#include <avr/interrupt.h>

// incorporate the PWM functionality from the earlier studio so that we can change the brightness of the LED as we bend the flex sensor. The ADC value must be used to change the PWM duty cycle so that we can observe a change in the brightness of the LED. The LED brightness must span the entire available range (from being very dim to very bright)

// Hint: First remap the 10-bit ADC value to the 8-bit OCR0A value. Next expand the range of this value based on the flex sensors readings you got earlier.

unsigned int adcvalue;
unsigned int remapped_adc;

void InitPWM() {
    TCCR0A = 0b00000001;
    TIMSK0 |= 0b10;

    OCR0A = 0;
    TCNT0 = 0;
}

void startPWM() {
    TCCR0B = 0b00000100;
}

ISR(TIMER0_COMPA_vect)
{
    // Provide your code for the ISR
}

ISR(ADC_vect)
{
    // Provide your code for the ISR
}

void setup() {
    cli();
    // put your setup code here, to run once:
    PRR &= 0b11111110;
    ADCSRA = 0b10001111;
    ADMUX = 0b01000000;
    DDRB |= 0b00100000;
    InitPWM();
    startPWM();
    sei();
    ADCSRA |= 0b01000000;
}

void loop() {}