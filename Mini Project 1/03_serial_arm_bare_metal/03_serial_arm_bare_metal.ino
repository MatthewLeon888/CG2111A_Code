// TAKE NOTE: Once you find the angle limit for each joint, 
// modify the following code to prevent the arm moving past these limits

// TAKE ANOTHER NOTE: The following code will use bare metal programming

// #define PIN7 (1 << 7)
// #define PIN6 (1 << 6)
// #define PIN5 (1 << 5)
// #define PIN4 (1 << 4)
// #define PIN3 (1 << 3)
// #define PIN2 (1 << 2)
// #define PIN1 (1 << 1)
// #define PIN0 (1 << 0)

// const int BASE_PIN     = 14; // A0
// const int SHOULDER_PIN = 15; // A1
// const int ELBOW_PIN    = 16; // A2
// const int GRIPPER_PIN  = 17; // A3

#define BASE_PIN     PC0 // 0b00000000, 0
#define SHOULDER_PIN PC1 // 0b00000001, 1
#define ELBOW_PIN    PC2 // 0b00000010, 2
#define GRIPPER_PIN  PC3 // 0b00000011, 3

// int basePos     = 90;
// int shoulderPos = 90;
// int elbowPos    = 90;
// int gripperPos  = 90;
int msPerDeg    = 10;

static volatile bool trigger = true;
static volatile int servoNum = 0;

int servos[4]     = {BASE_PIN, SHOULDER_PIN, ELBOW_PIN, GRIPPER_PIN};
static volatile int servoPos[4]   = {      90,           90,        90,          90};
static volatile int servoTicks[4] = {    20000,         20000,      20000,        20000};
//28000 to 12000 range now.

// Angles range: 0deg to 180deg
// Ticks  range: 1050  to  1950 (to maintain 1deg =  5ticks ratio)
// Ticks  range: 15980 to 12020 (to maintain 1deg = -22ticks ratio)

void setup() {
  Serial.begin(115200);

  // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTIONS
  // base.attach(BASE_PIN);
  // shoulder.attach(SHOULDER_PIN);
  // elbow.attach(ELBOW_PIN);
  // gripper.attach(GRIPPER_PIN);

  // NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTIONS
  // base.write(basePos);
  // shoulder.write(shoulderPos);
  // elbow.write(elbowPos);
  // gripper.write(gripperPos);

  cli();
  // SREG  &= ~0b10000000; // cli() alternative

  // Set Pins PC3, PC2, PC1, PC0 as Output
  // DDRC |= (1 << PC3) | (1 << PC2) | (1 << PC1) | (1 << PC0);
  DDRC  |= 0b00001111;

  // Output Mode: OC1A and OC1B are disconnected, COM1A1:0 = 00, COM1B1:0 = 00
  // Waveform Mode: PWM Phase Correct w ICR1 TOP, WGM13:10 = 1010
  TCCR1A = 0b00000010; 

  // Enable Compare Match A and B Interrupts, OCIE1B = 1, OCIE1A = 1
  TIMSK1 = 0b00000110;

  // Set up TOP
  ICR1   = 32000;

  // Set up Timer 1 counter
  // Recall: Timer 1 will count up from 0 to 20000(TOP) and then count down from 20000 to 0
  // TIMER1_COMPA_vect is triggered whenever TCNT1 = OCR1A
  TCNT1  = 0;

  // Set up duty cycle with OCR1A
  // Change OCR1A adjust duty cycles
  // Recall: 
  //   0deg servo ->   5% duty cycle -> OCR1A = 1000 (4000) (16000)
  //  90deg servo -> 7.5% duty cycle -> OCR1A = 1500 (6000) (14000)
  // 180deg servo ->  10% duty cycle -> OCR1A = 2000 (8000) (12000)
  OCR1A  = 20000;
  OCR1B  = 0;

  // Waveform Mode: WGM13:10 = 1010
  // Prescalar: 8 (For accuracy and maximum timer resolution), CS12:10 = 010
  TCCR1B = 0b00010001;

  sei();
  // SREG  |= 0b10000000; // sei() alternative


  homeAll();
}

// ISR(TIMER1_COMPA_vect) {
//   // If trigger is TRUE, Servo pins outputs ON
//   // else, Servo pins outputs OFF
//   // This interrupt will make the PWM signal for the servos
//   (trigger) ? PORTC |= 0b00001111 : PORTC &= ~0b00001111;

//   // Flips state of trigger from TRUE to FALSE or FALSE to TRUE
//   trigger = !trigger;
// }

ISR(TIMER1_COMPA_vect) {
  // (trigger) ? PORTC |= (1 << PC1) : PORTC &= ~(1 << PC1);
  (trigger) ? PORTC |= 0b00001111 : PORTC &= ~0b00001111;
  trigger = !trigger; 
  Serial.println(trigger);
}

ISR(TIMER1_COMPB_vect) {
  servoNum++;
  if (servoNum >= 4) servoNum = 0;
  Serial.println("sup");
  //OCR1A = servoTicks[servoNum];
}

int parse3(const String *s) {
  if (!s) return -1;
  if (s->length() != 3) return -1;
  if (!isDigit(s->charAt(0)) || !isDigit(s->charAt(1)) || !isDigit(s->charAt(2))) return -1;
  return (s->charAt(0) - '0') * 100 + (s->charAt(1) - '0') * 10 + (s->charAt(2) - '0');
}

// NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTION
// void moveSmooth(Servo *sv, int *cur, int target) {
//   if (!sv || !cur) return;

//   target = constrain(target, 0, 180);
//   int step = (target > *cur) ? 1 : -1;

//   while (*cur != target) {
//     *cur += step;
//     sv->write(*cur);
//     delay(msPerDeg);
//   }
// }
void move(int pin, int* cur, int* ticks, int target) {
  if (!cur) return;

  Serial.println(pin);

  target = constrain(target, 0, 180);
  int step = (target > *cur) ? 1 : -1;

  while (*cur != target) {
    *cur += step; //i degree maps to 80 something ticks?
    *ticks -= 20 * step;
    delay(msPerDeg);
  }
}

// NEED BARE METAL SUBSTITUTES FOR BELOW FUNCTIONS
void homeAll() {
  // moveSmooth(&base,     &basePos,     90);
  // moveSmooth(&shoulder, &shoulderPos, 90);
  // moveSmooth(&elbow,    &elbowPos,    90);
  // moveSmooth(&gripper,  &gripperPos,  90);
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
