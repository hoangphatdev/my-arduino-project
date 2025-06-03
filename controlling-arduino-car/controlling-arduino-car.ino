#include <PS2X_lib.h>
#include <Servo.h>

PS2X ps2x;

// PS2 pins
int ps2_data= 12;
int ps2_command = 11;
int ps2_attention = 10;
int ps2_clock = 13;
const bool pressures = false;
const bool vibration = false;

// PS2 button map
#define up PSB_PAD_UP
#define down PSB_PAD_DOWN
#define left PSB_PAD_LEFT
#define right PSB_PAD_RIGHT
#define pink PSB_PINK
#define red PSB_RED
#define green PSB_GREEN
#define blue PSB_BLUE
#define l1 PSB_L1
#define l2 PSB_L2
#define r1 PSB_R1
#define r2 PSB_R2

// Motor pins
int ena = 5;
int in1 = 2;
int in2 = 4;
int enb = 6;
int in3 = 7;
int in4 = 8;

// Servo objects
Servo servoBottom;
Servo servoHand;
Servo servoRight;
Servo servoLeft;

// Initial values
int initialServoBottomValue = 90;
int initialServoHandValue = 0;
int initialServoRightValue = 0;
int initialServoLeftValue = 0;

void setup() {
  Serial.begin(9600); // Thêm để xem debug trên Serial Monitor

  delay(1500);  // Wait for PS2 controller to initialize
  ps2x.config_gamepad(ps2_clock, ps2_command, ps2_attention, ps2_data, pressures, vibration);

  // Attach servos
  servoBottom.attach(A3);
  servoRight.attach(A2);
  servoLeft.attach(A5);
  servoHand.attach(A4);

  delay(400);
  servoRight.write(initialServoRightValue);
  servoBottom.write(initialServoBottomValue);
  servoHand.write(initialServoHandValue);
  servoLeft.write(initialServoLeftValue);

  // Setup motor pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  carStop();
}

void loop() {
  ps2x.read_gamepad(false, false);  // Read PS2 state

  // Movement
  if (ps2x.Button(up)) {
    carGoStraight();
  } else if (ps2x.Button(down)) {
    carGoBackward();
  } else if (ps2x.Button(right)) {
    carTurnRight();
  } else if (ps2x.Button(left)) {
    carTurnLeft();
  } else {
    carStop();
  }

  // Rotate base
  if (ps2x.Button(pink) && initialServoBottomValue <= 175) {
    initialServoBottomValue += 2;
    servoBottom.write(initialServoBottomValue);
    delay(20);
  }
  if (ps2x.Button(red) && initialServoBottomValue >= 10) {
    initialServoBottomValue -= 2;
    servoBottom.write(initialServoBottomValue);
    delay(20);
  }

  // Extend/retract arm
  if (ps2x.Button(l1) && initialServoRightValue >= 10) {
    initialServoRightValue -= 1;
    servoRight.write(initialServoRightValue);
    delay(10);
  }
  if (ps2x.Button(l2) && initialServoRightValue <= 100) {
    initialServoRightValue += 1;
    servoRight.write(initialServoRightValue);
    delay(10);
  }

  // Raise/lower arm
  if (ps2x.Button(blue) && initialServoLeftValue >= 10) {
    initialServoLeftValue -= 1;
    servoLeft.write(initialServoLeftValue);
    delay(10);
  }
  if (ps2x.Button(green) && initialServoLeftValue <= 90) {
    initialServoLeftValue += 1;
    servoLeft.write(initialServoLeftValue);
    delay(10);
  }

  // Open/close gripper
  if (ps2x.Button(r1) && initialServoHandValue <= 90) {
    initialServoHandValue += 2;
    servoHand.write(initialServoHandValue);
    delay(20);
  }
  if (ps2x.Button(r2) && initialServoHandValue >= 10) {
    initialServoHandValue -= 2;
    servoHand.write(initialServoHandValue);
    delay(20);
  }
}

void carGoStraight() {
  Serial.println("Moving Forward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 240);
  analogWrite(enb, 240);
}

void carGoBackward() {
  Serial.println("Moving Backward");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, 240);
  analogWrite(enb, 240);
}

void carTurnLeft() {
  Serial.println("Turning Left");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, 100);
  analogWrite(enb, 240);
}

void carTurnRight() {
  Serial.println("Turning Right");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, 240);
  analogWrite(enb, 100);
}

void carStop() {
  Serial.println("Stopping");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}
