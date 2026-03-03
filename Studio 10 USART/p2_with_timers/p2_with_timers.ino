#include <avr/io.h>
#include <avr/interrupt.h>

/*#ifndef F_CPU
#define F_CPU   16000000UL
#endif
*/
#include <util/delay.h>

// Masks for pins 12 and 11
#define PIN12MASK   0b00010000
#define PIN11MASK   0b00001000

volatile char flashWhich = 1;
volatile char buttonVal = 1;

// These variables are shared between ISRs and main loop
// MUST be declared volatile to prevent compiler optimization issues
volatile char dataRecv, dataSend;

static volatile unsigned long _timerTicks = 0;

ISR(USART_RX_vect) {
  dataRecv = UDR0;
}

ISR(USART_UDRE_vect) {
  UDR0 = dataSend;
  UCSR0B &= ~0b00100000;
}

// Note that we're not using the USART TX interrupt here because we're not intending
// to take any action immediately after data being sent
// Example use case: putting the 328P into a sleep mode after sending data.

void sendData(const char data) {
  // Copy data to be sent to dataSend
  dataSend = data;

  // Enable UDRE interrupt below
  UCSR0B |= 0b00100000;
}

char recvData() {
  return dataRecv - '0';
}

void setupSerial() {
  // Set up the 9600 8N1 when using Serial Monitor to test
  // UCSR0C = 0b00000110;

  // Change to 9600 7E1 when communicating between Arduinos.
  UCSR0C = 0b00100100;

  UBRR0L = 103;
  UBRR0H = 0;
  UCSR0A = 0;
}

void startSerial() {
  // Start the serial port.
  // Enable RXC interrupt, but NOT UDRIE
  // Remember to enable the receiver and transmitter
  UCSR0B = 0b10011000;
}

// Enable external interrupts 0 and 1
void setupEINT() {
  // Configure INT0 and INT1 for rising edge triggered.
  // Remember to enable INT0 and INT1 interrupts.
  EICRA |= 0b00001111;
  EIMSK |= 0b00000011;
}

// ISRs for external interrupts
ISR(INT0_vect) {
  buttonVal = 1;
  // TODO: call sendData correctly
  sendData('0'+ buttonVal);
}

ISR(INT1_vect) {
  buttonVal = 2;
  // TODO: call sendData correctly
  sendData('0' + buttonVal);
}

ISR(TIMER2_COMPA_vect) {
  _timerTicks++;
}

void wait_n(unsigned long n) {
  unsigned long _currentTime = _timerTicks;
  // polling-ish wait loop
  while(_timerTicks - _currentTime < n*100);
}

// Red is on pin 12
void flashRed() {
  PORTB |= PIN12MASK;
  wait_n(100);
  PORTB &= ~PIN12MASK;
  wait_n(100);
}

// Green is on pin 11
void flashGreen() {
  PORTB |= PIN11MASK;
  wait_n(100);
  PORTB &= ~PIN11MASK;
  wait_n(100);
}

void setup() {
  cli();
  DDRB |= (PIN11MASK | PIN12MASK);
  setupEINT();
  setupSerial();
  startSerial();
  TCCR2A = 0b00000010;
  TIMSK2 = 0b10;
  TCNT2 = 0;
  OCR2A = 199;
  
  TCCR2B = 0b00000010; 
  sei();
}

void loop() {
  flashWhich = recvData();
   
  if (flashWhich == 1) {
    flashRed();
  } else {
    flashGreen();
  }
}
