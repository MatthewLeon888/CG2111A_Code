#define REDPIN     11
#define GREENPIN   12
#define SWITCHPIN  2
#define LED_DELAY  100
// This variable decides which LED's turn it is to flash.
// 0 = green, 1 = red
static volatile int turn = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  //pinMode(SWITCHPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SWITCHPIN), toggle, RISING);
}

void toggle() {
  turn = 1 - turn;
}

void flashGreen() {
  int counter = 1;
  while(turn == 0) {
    // This "for loop" is in charge of the flashing LED
    for(int i = 0; i < counter && turn == 0; i++) {
      digitalWrite(GREENPIN, HIGH);
      delay(LED_DELAY);
      digitalWrite(GREENPIN, LOW);
      delay(LED_DELAY);
    }
    // The counter is in charge of how many times the LED blinks
    // The longer the switch is NOT changed, the more times the LED blinks
    counter ++;
    for (int i=0; i<5; i++) {
      if (turn==1) return;
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
      digitalWrite(REDPIN, LOW);
      delay(LED_DELAY);
    }
    // The counter is in charge of how many times the LED blinks
    // The longer the switch is NOT changed, the more times the LED blinks
    counter ++;
    for (int i=0; i<5; i++) {
      if (turn==0) return;
      delay(200);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  if(turn == 0) flashGreen();
  if(turn == 1) flashRed();
}

// Question 5: Which of the polling version or interrupt version seems better to you? 
//             Examine the code and explain why. Any reasonable justification is acceptable. 
// Interrupt version is better since it is more responsive.
// Moreover, the interrupt version is simplier to implement as there is less edge cases to consider