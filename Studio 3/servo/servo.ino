#include <Servo.h>

const uint8_t SERVO_PIN = 2;

Servo servo;

void setup() {
  // Sets up the servo
  servo.attach(SERVO_PIN);
  // Forces it to be at its midpoint
  servo.write(90);
}

void loop() {
// Sweep 0° -> 180° -> 0°
// for (int angle = 0; angle <= 180; angle += 1) {
// servo.write(angle);
// delay(15);
// }
// for (int angle = 180; angle >= 0; angle -= 1) {
// servo.write(angle);
// delay(15);
// }
}
