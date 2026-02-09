#include "Arduino.h"

#define PIN5 (1 << 5) // 0b 0000 0001 to 0b 0010 0000 (0x1 to 0x20)
#define PIN4 (1 << 4) // 0b 0000 0001 to 0b 0001 0000 (0x1 to 0x10)

void setup() {
  // PIN5 = 0b 0010 0000
  // PIN4 = 0b 0001 0000
  DDRB = (PIN5 | PIN4);
  // DDRB = 0b 0011 0000
  PORTB = PIN5;
}

void loop() {
  // PIN5  = 0b 0010 0000
  //PORTB = PIN5; // Toggle Green LED
  // PORTB = 0b 0010 0000
  //delay(1000);

  // PIN4  = 0b 0001 0000
  //PORTB = PIN4; // Toggle Red LED
  // PORTB = 0b 0001 0000
  //delay(1000);

  // PORTB = 0b 0010 0000
  // PIN5  = 0b 0010 0000
  // PIN4  = 0b 0001 0000
  // DDRB  = 0b 0011 0000

  PORTB = PORTB ^ DDRB;
  delay(1000);
}
