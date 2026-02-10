// TAKE NOTE: Once you find the angle limit for each joint, 
// modify the following code to prevent the arm moving past these limits

// TAKE ANOTHER NOTE: The following code will use bare metal programming

#define BASE_PIN     PC0 // 0
#define SHOULDER_PIN PC1 // 1
#define ELBOW_PIN    PC2 // 2
#define GRIPPER_PIN  PC3 // 3

static volatile int msPerDeg      = 10;
static volatile int servoNo       = 0;
static volatile int servos[4]     = {BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN};
static volatile int servoPos[4]   = {      90,           90,        90,          90};
static volatile int servoTicks[4] = {    3000,         3000,      3000,        3000};

// Angles range: 0deg to 180deg
// Ticks  range: 2010 to 3990 (to maintain 1deg = 11 ticks)

void setup() {
  Serial.begin(115200);
  Serial.println(BASE_PIN);
  Serial.println(SHOULDER_PIN);
  Serial.println(ELBOW_PIN);
  Serial.println(GRIPPER_PIN);
  Serial.println(PC0);
  Serial.println(PC1);
  Serial.println(PC2);
  Serial.println(PC3);

  // Disable Interrupts
  cli();

  // Sets PC0, PC1, PC2 and PC3 as Output
  DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

  // Output Mode: OC1A and OC1B are disconnected, COM1A1:0 = 00, COM1B1:0 = 00
  // Waveform Mode: Fast PWM w ICRA as TOP, WGM13:10 = 1110
  TCCR1A = 0b00000010; 

  // Enable Compare Match A and B Interrupts
  TIMSK1 = 0b00000110;

  // Set up Timer 1 counter
  TCNT1  = 0;

  // Set up TOP
  ICR1   = 40000;

  // Set up pulse width
  OCR1A  = 3000;
  OCR1B  = 40000;

  // Waveform Mode: WGM13:10 = 1110
  // Prescalar: 8 (For accuracy and maximum timer resolution), CS12:10 = 010
  // Start Timer 1
  TCCR1B = 0b00011010;

  // Enable Interrupts
  sei();
}

ISR(TIMER1_COMPA_vect) {
  // Turn OFF ith servo
  PORTC &= ~(1 << servos[servoNo]);

  if (servoNo < 3) {
    // Increment servoNo
    servoNo++;

    // Turn ON next servo
    PORTC |= (1 << servos[servoNo]);

    // Update OCR1A
    OCR1A = TCNT1 + servos[servoNo];
  }
}

ISR(TIMER1_COMPB_vect) {
  // Reset servoNo
  servoNo = 0;

  // Turn ON first servo
  PORTC |= (1 << servos[servoNo]);

  // Reset OCR1A
  OCR1A = servos[servoNo];
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
    // 1deg maps to 11 ticks
    *ticks += 11 * step;
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
