#define buttonPin  2
#define LEDPIN     12
static volatile int onOff = 0;

void switchISR() {
  onOff = 1 - onOff;
}

void setup() {
  // put your setup code here, to run once:
  // Question 2: Explain what the attachInterrupt function does and what arguments it takes.
  // attachInterrupt sets up an interupt in a certain pin
  // In this case, the first parameter is the pin to be used in the interupt (buttonPin)
  // The second parameter is the function to call as soon as the interupt pin is triggerd (swtichISR)
  // The third parameter is the conditions needed for the interupt to trigger (RISING)
  // In this case, when the buttonPin goes from LOW to HIGH, the function switchISR is immeditately run
  // RISING means LOW to HIGH
  // LOW     → Triggers when pin is LOW.
  // CHANGE  → Triggers when pin changes from HIGH to LOW or LOW to HIGH
  // RISING  → Triggers when pin goes from LOW to HIGH.
  // FALLING → Triggers when pin goes from HIGH to LOW.
  // HIGH    → Triggers when pin is HIGH.
  attachInterrupt(digitalPinToInterrupt(buttonPin), switchISR, RISING);
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(onOff == 0) digitalWrite(LEDPIN, LOW);
  else digitalWrite(LEDPIN, HIGH);
}

// Question 3: Explain how your sketch is able to switch the LED on and off when you press the button.
// When the button is pressed, the pin it is connected to (buttonPin) will go from LOW to HIGH
// Since the pin goes from LOW to HIGH, the RISING condition is met and the switchISR() function is IMMEDIATELY run
// swtichISR() will toggle the state for onOff from either 0 to 1 or 1 to 0
// Finally, based on the void loop(), if the state of onOff was changed from 0 to 1, 
// the else condition runs and the output of LEDPIN is changed from LOW to HIGH (ie LED turns OFF)
// Similarly, if the state of onOff was changed from 1 to 0, the if condition is satisfied and runs,
// and the output of LEDPIN is changed from HIGH to LOW (ie LED turns ON)
