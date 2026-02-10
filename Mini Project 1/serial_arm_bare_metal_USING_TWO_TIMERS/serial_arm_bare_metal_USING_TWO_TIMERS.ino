// TAKE NOTE: Once you find the angle limit for each joint, 
// modify the following code to prevent the arm moving past these limits

// TAKE ANOTHER NOTE: The following code will use bare metal programming

#define BASE_PIN     PC0 // 0
#define SHOULDER_PIN PC1 // 1
#define ELBOW_PIN    PC2 // 2
#define GRIPPER_PIN  PC3 // 3

int msPerDeg    = 10;

static volatile int servoNum = 0;

int servos[4]                     = {BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN};
static volatile int servoPos[4]   = {      90,           90,        90,          90};
static volatile int servoTicks[4] = {   20000,        20000,     20000,       20000};

// Angles range: 0deg to 180deg
// Ticks  range: 

void setup() {
  Serial.begin(115200);

  // Disable Interrupts
  cli();

  // Set Pins PB1 and PB2 as Output for OCR1A and OCR1B respectively
  DDRB |= (1 << PB1) | (1 << PB2);

  // Sets PB3 and PD3 as Output for OC2A and OC2B respectively
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);

  // Output Mode: Clear OC1A and OC1B on compare match, COM1A1:0 = 10, COM1B1:0 = 10
  // Waveform Mode: Fast PWM w ICRA as TOP, WGM13:10 = 1110
  TCCR1A = 0b10100010; 

  // Output Mode: Clear OC2A and OC2B on compare match, COM2A1:0 = 10, COM2B1:0 = 10
  // Waveform Mode: Fast PWM w 0xFF as TOP, WGM22:20 = 011
  TCCR2A = 0b10100011;

  // Disable Timer 1 and 2 Interrupts
  TIMSK1 = 0b00000000;
  TIMSK2 = 0b00000000;

  // Set up Timer 1 and 2 counter
  TCNT1  = 0;
  TCNT2  = 0;

  // Set up Timer 1 TOP
  ICRA   = 40000;

  // Set up pulse width for servos at PB1, PB2, PB3 and PD3
  OCR1A  = 3000;
  OCR1B  = 3000;
  OCR2A  = 19;
  OCR2B  = 19;

  // Waveform Mode: WGM13:10 = 1110
  // Prescalar: 8 (For accuracy and maximum timer resolution), CS12:10 = 010
  // Start Timer 1
  TCCR1B = 0b00011001;

  // Waveform Mode: WGM22:20 = 011
  // Prescalar: CS12:10 = ...
  // Start Timer 2
  TCCR1B = 0b00000001;

  // Enable Interrupts
  sei();
}

ISR(TIMER1_OVF_vect) {
  // Software based PWM
  // Set SHOULDER (PB3) and ELBOW (PB4) servos
  PORTD |= (1 << PB4) | (1 << PB3);
  
}

int parse3(const String *s) {
  if (!s) return -1;
  if (s->length() != 3) return -1;
  if (!isDigit(s->charAt(0)) || !isDigit(s->charAt(1)) || !isDigit(s->charAt(2))) return -1;
  return (s->charAt(0) - '0') * 100 + (s->charAt(1) - '0') * 10 + (s->charAt(2) - '0');
}

void move(int pin, int* cur, int* ticks, int target) {
  if (!cur) return;

  Serial.println(pin);

  target = constrain(target, 0, 180);
  int step = (target > *cur) ? 1 : -1;

  while (*cur != target) {
    *cur += step; 
    // 1deg maps to 80 ticks
    *ticks -= 20 * step;
    delay(msPerDeg);
  }
}

void homeAll() {
  move(BASE_PIN,     &servoPos[BASE_PIN],     &servoTicks[BASE_PIN],     90);
  move(SHOULDER_PIN, &servoPos[SHOULDER_PIN], &servoTicks[SHOULDER_PIN], 90);
  move(ELBOW_PIN,    &servoPos[ELBOW_PIN],    &servoTicks[ELBOW_PIN],    90);
  move(GRIPPER_PIN,  &servoPos[GRIPPER_PIN],  &servoTicks[GRIPPER_PIN],  90);
}

void loop() {
  // if (!Serial.available()) return;

  // // Reads a string with the command until newline
  // String cmd = Serial.readStringUntil('\n');
  // cmd.trim(); // Remove any extra whitespace
  // if (!cmd.length()) return; // didn't read anything

  // // Handle the home command
  // if (cmd == "H") {
  //   Serial.println("Homing all servos...");
  //   homeAll();
  //   return;
  // }

  // // All subsequent commands need to have 4 characters
  // if (cmd.length() != 4) {
  //   Serial.println("ERROR: Command is not 4 characters long");
  //   return;
  // }

  // // c is now the command character
  // char c = cmd.charAt(0);
  // // val is the numerical value of the argument
  // int val = parse3(&cmd.substring(1));
  // if (val < 0) { 
  //   Serial.println("ERROR: Argument not valid");
  //   return;
  // }

  // // Vddd sets velocity as ms per degree
  // if (c == 'V') {
  //   Serial.print("Setting velocity to ");
  //   Serial.println(val);
  //   msPerDeg = val;
  //   return;
  // } else if (c == 'B') {
  //   Serial.print("Moving base to ");
  //   Serial.println(val);
  //   // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTION
  //   // moveSmooth(&base, &basePos, val);
  //   move(BASE_PIN, &servoPos[BASE_PIN], &servoTicks[BASE_PIN], val);
  //   return;
  // } else if (c == 'S') {
  //   Serial.print("Moving shoulder to ");
  //   Serial.println(val);
  //   // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTION
  //   // moveSmooth(&shoulder, &shoulderPos, val);
  //   move(SHOULDER_PIN, &servoPos[SHOULDER_PIN], &servoTicks[SHOULDER_PIN], val);
  //   return;
  // } else if (c == 'E') {
  //   Serial.print("Moving elbow to ");
  //   Serial.println(val);
  //   // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTION
  //   // moveSmooth(&elbow, &elbowPos, val);
  //   move(ELBOW_PIN, &servoPos[ELBOW_PIN], &servoTicks[ELBOW_PIN], val);
  //   return;
  // } else if (c == 'G') {
  //   Serial.print("Moving gripper to ");
  //   Serial.println(val);
  //   // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTION
  //   // moveSmooth(&gripper, &gripperPos, val);
  //   move(GRIPPER_PIN, &servoPos[GRIPPER_PIN], &servoTicks[GRIPPER_PIN], val);
  //   return;
  // } else {
  //   Serial.println("ERROR: Unknown command");
  //   return;
  // }
}
