#define REDPIN     11
#define GREENPIN   12
#define SWITCHPIN  2
#define LED_DELAY  100
// This variable decides which LED's turn it is to flash.
// 0 = green, 1 = red
static volatile int turn = 0;

void toggle() {
  turn = 1 - turn;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(SWITCHPIN, INPUT);
}

void flashGreen() {
  int counter = 1;
  while(turn == 0) {
    // This "for loop" is in charge of the flash LED
    for(int i = 0; i < counter && turn == 0; i++) {
      digitalWrite(GREENPIN, HIGH);
      delay(LED_DELAY);
      //if(digitalRead(SWITCHPIN)) {toggle(); return;}
      digitalWrite(GREENPIN, LOW);
      delay(LED_DELAY);
      if(digitalRead(SWITCHPIN)) {toggle(); return;}
    }
    // The counter is in charge of how many times the LED blinks
    // The longer the switch is NOT changed, the more times the LED blinks
    counter++;
    for (int i=0; i<5; i++) {
      if(digitalRead(SWITCHPIN)) {toggle(); return;}
      delay(200);
    }
  }
}

void flashRed() {
  int counter = 1;
  while(turn == 1) {
    // This "for loop" is in charge of the flash LED
    for(int i = 0; i < counter && turn == 1; i++) {
      digitalWrite(REDPIN, HIGH);
      delay(LED_DELAY);
      //if(digitalRead(SWITCHPIN)) {toggle(); return;}
      digitalWrite(REDPIN, LOW);
      delay(LED_DELAY);
      if(digitalRead(SWITCHPIN)) {toggle(); return;}
    }
    // The counter is in charge of how many times the LED blinks
    // The longer the switch is NOT changed, the more times the LED blinks
    counter++;
    for (int i=0; i<5; i++) {
      if(digitalRead(SWITCHPIN)) {toggle(); return;}
      delay(200);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(turn == 0) flashGreen();
  if(turn == 1) flashRed();
}