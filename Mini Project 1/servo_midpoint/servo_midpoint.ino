#include <Servo.h>

const int BASE_PIN     = 14; // A0
const int SHOULDER_PIN = 15; // A1
const int ELBOW_PIN    = 16; // A2
const int GRIPPER_PIN  = 17; // A3

Servo base, shoulder, elbow, gripper;

int basePos = 90, shoulderPos = 90, elbowPos = 90, gripperPos = 90;

void setup() {
  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  gripper.attach(GRIPPER_PIN);

  base.write(basePos);
  shoulder.write(shoulderPos);
  elbow.write(elbowPos);
  gripper.write(gripperPos);
}

void loop() {}
