// NOTE: The following code uses bare metal programming

#define BASE_PIN     PC0
#define SHOULDER_PIN PC1
#define ELBOW_PIN    PC2
#define GRIPPER_PIN  PC3

#define CONVERSION_RATE 22 // Conversion rate where 1deg = 17 ticks
#define BUFFER_RANGE     2 // Buffer range for updating servoPos

// static volatile int msPerDeg      = 10; // Changes speed of rotation (lower is faster)
static volatile int angularSpeed  = 1;  // Changes speed of rotation (higher is faster)
static volatile int servoNo       = 0;  // Index of each servo
static volatile int servoPos[4]   = {  90,   90,   90,   90};
static volatile int newPos[4]     = {  90,   90,   90,   90};
static volatile int servoTicks[4] = {3000, 3000, 3000, 3000};

//   0deg = 1000 Ticks
//  90deg = 3000 Ticks
// 180deg = 5000 Ticks

// Angles range: 0deg to 180deg
// Ticks  range: 1020 to 4980 (to maintain 1deg = 22 ticks)

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

  // Set up TOP as 0.005s (4 cycles for period and frequency to be 0.02s and 50Hz respectively)
  ICR1   = 10000;

  // Set up pulse width
  // One period = 40000 ticks
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
  if (newPos[servoNo] - servoPos[servoNo] > angularSpeed * BUFFER_RANGE) {
    servoPos[servoNo]   += angularSpeed;
    servoTicks[servoNo] += angularSpeed * CONVERSION_RATE;
  } else if (newPos[servoNo] - servoPos[servoNo] < angularSpeed * BUFFER_RANGE) {
    servoPos[servoNo]   -= angularSpeed;
    servoTicks[servoNo] -= angularSpeed * CONVERSION_RATE;
  }
  
  // // Update servoPos and servoTicks
  // if (newPos[servoNo] - servoPos[servoNo] > BUFFER_RANGE) {
  //   servoPos[servoNo]   += angularSpeed;
  //   servoTicks[servoNo] += angularSpeed * CONVERSION_RATE;
  // } else if (newPos[servoNo] - servoPos[servoNo] < BUFFER_RANGE) {
  //   servoPos[servoNo]   -= angularSpeed;
  //   servoTicks[servoNo] -= angularSpeed * CONVERSION_RATE;
  // }

  // Update OCR1B
  OCR1B = servoTicks[servoNo];

  // Turn ON ith servo
  PORTC |= (1 << servoNo);
}

ISR(TIMER1_COMPB_vect) {
  // Turn OFF ith servo
  PORTC &= ~(1 << servoNo);
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

  // int step = (servoPov[pin] < target) ? 1 : -1;
  // while (servoPov[pin] != target) {
  //   servoPin[pin]   += step; 
  //   servoTicks[pin] += step * CONVERSION_RATE;
  //   delay(msPerDeg);
  // }
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

  // Refresh angularSpeed if motor makes sound
  if (cmd == "R") {
    Serial.println("Refresh...");
    int temp = angularSpeed;
    angularSpeed = 1;
    delay(500);
    angularSpeed = temp;
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

  switch (c) {
    case 'V':
      val = constrain(val, 0, BUFFER_RANGE);
      Serial.print("Setting velocity to ");
      Serial.println(val);
      angularSpeed = val;
      break;
    case 'B':
      val = constrain(val, 10, 170);
      Serial.print("Moving base to ");
      Serial.println(val);
      move(BASE_PIN, val);
      break;
    case 'S': 
      val = constrain(val, 110, 180);
      Serial.print("Moving shoulder to ");
      Serial.println(val);
      move(SHOULDER_PIN, val);
      break;
    case 'E':
      val = constrain(val, 70, 160);
      Serial.print("Moving elbow to ");
      Serial.println(val);
      move(ELBOW_PIN, val);
      break;
    case 'G':
      val = constrain(val, 55, 90);
      Serial.print("Moving gripper to ");
      Serial.println(val);
      move(GRIPPER_PIN, val);
      break;
      case 'T':
      Serial.print("Ticks value set to ");
      Serial.println(val * 10);
      servoTicks[BASE_PIN] = val * 10;
      break;
    default:
      Serial.println("ERROR: Unknown command");
      break;
  }

  return;
}