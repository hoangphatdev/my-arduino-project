/*
  motor trái: 3->4
  motor phải: 2->1

  Hướng dẫn điều khiển motor:
    Ở một cụm motor(ví dụ được IN1,IN2 quản lý), thì để động cơ chạy thì 1 input nhận giá trị HIGH(1), 1 input nhận giá trị LOW(0).
    Để điều khiển chiều thì đổi ngược lại HIGH LOW
*/
const int in1= A4;
const int in2= A5;
const int in3=2;
const int in4=3;
const int ena=5;
const int enb=6;

const int LouterSen=8;
const int LinnerSen=9;
const int midSen=10;
const int RinnerSen=11;
const int RouterSen=12;

int s1,s2,s3,s4,s5;
void setup() {
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(enb,OUTPUT);

  pinMode(LouterSen,INPUT);
  pinMode(LinnerSen,INPUT);
  pinMode(midSen,INPUT);
  pinMode(RinnerSen,INPUT);
  pinMode(RouterSen, INPUT);

}


// Trọng số cho từng cảm biến từ trái sang phải
// Cảm biến ngoài cùng có trọng số lớn hơn
const int weights[5] = {-4, -2, 0, 2, 4}; 
float Kp = 80; 
float Ki = 0;
float Kd = 20; 

float error = 0;
float lastError = 0;
float integral = 0;

int baseSpeed = 120;

void loop() {
  readSensor();

  int sensors[5] = {s1, s2, s3, s4, s5};
  int activeCount = 0;
  int position = 0;

  for (int i = 0; i < 5; i++) {
    if (sensors[i] == 1) {
      position += weights[i];
      activeCount++;
    }
  }


if (activeCount == 0) {
 if (lastError > 0.2) {
  turnRightStrong(250, 250);
  // delay( 100);
} else if (lastError < -0.2) {
  turnLeftStrong(250, 250);
  // delay(100);
}else{
  forward(100,100);
}
  return;
}


  error = position / activeCount;

  integral += error;
  float derivative = error - lastError;
  float correction = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

//  if (abs(error) > 3) {
//   baseSpeed = 60; // đủ chậm để rẽ, nhưng không quá yếu
// } else if (abs(error) > 1) {
//   baseSpeed = 50;
// } else {
//   baseSpeed = 100;
// }
  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;


  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  forward(leftSpeed, rightSpeed);

  delay(10); // Delay nhỏ để mượt hơn

}

void readSensor(){
  s1 = digitalRead(LouterSen);
  s2 = digitalRead(LinnerSen);
  s3 = digitalRead(midSen);
  s4 = digitalRead(RinnerSen);
  s5 = digitalRead(RouterSen);
}

/*
  motor trái: 3->4
  motor phải: 2->1
*/
void forward(int Lspeed, int Rspeed){
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  analogWrite(ena, Rspeed);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enb, Lspeed);

}
void backward(int Lspeed, int Rspeed){
  digitalWrite(in2, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(ena, Rspeed);
  analogWrite(enb, Lspeed);
}

void turnLeftStrong(int Lspeed,int Rspeed){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, HIGH);
  digitalWrite(in3, LOW);
  analogWrite(ena, Rspeed);
  analogWrite(enb, Lspeed);
}
void turnLeftWeak(int Lspeed,int Rspeed){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  analogWrite(ena, Rspeed);
  analogWrite(enb, Lspeed);
}
void turnRightStrong(int Lspeed,int Rspeed){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);

   analogWrite(ena, Rspeed);
  analogWrite(enb, Lspeed);
}
void turnRightWeak(int Lspeed, int Rspeed){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  analogWrite(ena, Rspeed);
  analogWrite(enb, Lspeed);
}


