// TAKE NOTE: Once you find the angle limit for each joint, 
// modify the following code to prevent the arm moving past these limits

// TAKE ANOTHER NOTE: The following code will use bare metal programming

#define BASE_PIN     PC0
#define SHOULDER_PIN PC1
#define ELBOW_PIN    PC2
#define GRIPPER_PIN  PC3

#define CONVERSION_RATE 17 // Conversion rate where 1deg = 17 ticks
#define BUFFER_RANGE    10 // Buffer range for updating servoPos and servoTicks

static volatile int angularSpeed  = 1; 
static volatile int servoNo       = 0;
static volatile int servoPos[4]   = {   0,   90,  180,   90};
static volatile int newPos[4]     = {   0,   90,  180,   90};
static volatile int servoTicks[4] = {1530, 3030, 4530, 3030};

// Angles range: 0deg to 180deg
// Ticks  range: 1500 to 4560 (to maintain 1deg = 17 ticks)

void setup() {
  Serial.begin(115200);

  // Disable Interrupts
  cli();

  // Sets PC0, PC1, PC2 and PC3 as Output
  DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

  // Turn OFF PC0, PC1, PC2 and PC3 as precaution
  PORTC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));

  // Output Mode: OC1A and OC1B are disconnected, COM1A1:0 = 00, COM1B1:0 = 00
  // Waveform Mode: CTC w IRC1 as TOP, WGM13:10 = 1100
  TCCR1A = 0b00000000; 

  // Enable Compare Match A and B Interrupts
  TIMSK1 = 0b00000110;

  // Set up Timer 1 counter
  TCNT1  = 0;

  // Set up TOP
  ICR1   = 10000;

  // Set up pulse width
  // One period = 40000
  OCR1A  = 0;
  OCR1B  = 3000;

  // Waveform Mode: WGM13:10 = 1100
  // Prescalar: 8 (For accuracy and maximum timer resolution), CS12:10 = 010
  // Start Timer 1
  TCCR1B = 0b00011010;

  // Enable Interrupts
  sei();

  Serial.println("READY");
}

ISR(TIMER1_COMPA_vect) {
  // Increment servoNo
  servoNo = (servoNo + 1) % 4;

  // Update servoPos and servoTicks
  if (newPos[servoNo] - servoPos[servoNo] > BUFFER_RANGE) {
    servoPos[servoNo]   += angularSpeed;
    servoTicks[servoNo] += angularSpeed * CONVERSION_RATE;
  } else if (newPos[servoNo] - servoPos[servoNo] < BUFFER_RANGE) {
    servoPos[servoNo]   -= angularSpeed;
    servoTicks[servoNo] -= angularSpeed * CONVERSION_RATE;
  }

  // Update OCR1B
  OCR1B = servoTicks[servoNo];

  // Turn ON ith servo
  PORTC |= (1 << servoNo);

  // Serial.println(TCNT1);
  // Serial.print("Servo ON: ");
  // Serial.println(servoNo);
}

ISR(TIMER1_COMPB_vect) {
  // Turn OFF ith servo
  PORTC &= ~(1 << servoNo);

  // Serial.println(TCNT1);
  // Serial.print("Servo OFF: ");
  // Serial.println(servoNo);
  // Serial.println();
}

int parse3(const String *s) {
  if (!s) return -1;
  if (s->length() != 3) return -1;
  if (!isDigit(s->charAt(0)) || !isDigit(s->charAt(1)) || !isDigit(s->charAt(2))) return -1;
  return (s->charAt(0) - '0') * 100 + (s->charAt(1) - '0') * 10 + (s->charAt(2) - '0');
}

void move(int pin, int target) {
  target = constrain(target, 0, 180);
  newPos[pin] = target;
}

void homeAll() {
  move(BASE_PIN,     90);
  move(SHOULDER_PIN, 90);
  move(ELBOW_PIN,    90);
  move(GRIPPER_PIN,  90);
}

void loop() {
  if (!Serial.available()) return;

  // Reads a string with the command until newline
  String cmd = Serial.readStringUntil('\n');
  cmd.trim(); // Remove any extra whitespace
  if (!cmd.length()) return; // didn't read anything

  // Handle the home command
  if (cmd == "H") {
    Serial.println("Homing all servos...");
    homeAll();
    return;
  }

  // All subsequent commands need to have 4 characters
  if (cmd.length() != 4) {
    Serial.println("ERROR: Command is not 4 characters long");
    return;
  }

  // c is now the command character
  char c = cmd.charAt(0);
  // val is the numerical value of the argument
  int val = parse3(&cmd.substring(1));
  if (val < 0) { 
    Serial.println("ERROR: Argument not valid");
    return;
  }

  // Vddd sets velocity as ms per degree
  if (c == 'V') {
    Serial.print("Setting velocity to ");
    val = constrain(val, 0, BUFFER);
    Serial.println(val);
    // msPerDeg = val;
    angularSpeed = val;
    return;
  } else if (c == 'B') {
    Serial.print("Moving base to ");
    val = constrain(val, 0, 180);
    Serial.println(val);
    move(BASE_PIN, val);
    return;
  } else if (c == 'S') {
    Serial.print("Moving shoulder to ");
    val = constrain(val, 110, 180);
    Serial.println(val);
    move(SHOULDER_PIN, val);
    return;
  } else if (c == 'E') {
    Serial.print("Moving elbow to ");
    val = constrain(val, 50, 180);
    Serial.println(val);
    move(ELBOW_PIN, val);
    return;
  } else if (c == 'G') {
    Serial.print("Moving gripper to ");
    val = constrain(val, 55, 90);
    Serial.println(val);
    move(GRIPPER_PIN, val);
    return;
  } else {
    Serial.println("ERROR: Unknown command");
    return;
  }
}
