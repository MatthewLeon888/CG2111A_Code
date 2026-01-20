/*
 * w5s1p3
 *
 * Created: 2/2/2018 6:01:01 AM
 * Author : dcstanc
 */ 

#include <avr/io.h>

// Remember to #include <avr/interrupt.h>

#include <util/delay.h>

#define LED_DELAY  100

static volatile int turn=0;

void flashGreen()
{
  int count=1;
  int i;

  while(turn==0)
  {
    for(i=0; i<count && turn==0; i++)
    {
      // Switch green LED at pin 12 on. Pin 12 is PB4
      PORTB |= 0b00010000;
    
      // Delay 250ms
      _delay_ms(LED_DELAY);
    
      PORTB &= 0b11101111;
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

  while(turn==1)
  {
    for(i=0; i<count && turn==1; i++)
    {
      // Switch green LED at pin 12 on. Pin 12 is PB4
      PORTB |= 0b00001000;
      
      // Delay 250ms
      _delay_ms(LED_DELAY);
      
      PORTB &= 0b11110111;
      _delay_ms(LED_DELAY);
    }
    
    _delay_ms(1000);
    count++;
  }
}

void setup()
{
  // Set pins 11 and 12 to output. Pin 11 is PB3, pin 12 is PB4
  DDRB |= 0b00011000;
}

void loop()
{
  if(turn==0)
    flashGreen();
  else
    flashRed();
}
