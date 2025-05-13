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

const int trigPin = A2;
const int echoPin = A3;

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

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

Serial.begin(9600);
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

long duration;
long distance;

unsigned long lastUltrasonicRead = 0;
const unsigned long ultrasonicInterval = 100; // 100ms

int currentSpeed = 0;

void loop() {
  if (millis() - lastUltrasonicRead >= ultrasonicInterval) {
    lastUltrasonicRead = millis();
    readUltrasonic();
  }

  if (distance > 0 && distance < 20) { // Nếu phát hiện bậc thang (khoảng cách gần)
   Serial.println("gap vat can");
    stopMotors();
    delay(200);          // Dừng 300ms
    fullSpeedForward();  // Tăng tốc mạnh
    delay(1000);         // Chạy mạnh 1.5 giây để leo bậc
    return;              // Sau đó quay lại follow line
  }
  readSensor();

  // int sumSensor = s1 + s2 + s3 + s4 + s5;
  // if(sumSensor == 5){
  //   stopMotors();
  //   while(true);
  // }
  
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

void accelerateForward() {
  for (currentSpeed = 0; currentSpeed < baseSpeed; currentSpeed++) {
    forward(currentSpeed, currentSpeed); // Điều khiển động cơ
    delay(10); // Thời gian nhỏ để tăng tốc từ từ
  }
}
void fullSpeedForward(){
  forward(255,255);
}

// ------ Đọc cảm biến siêu âm ------
void readUltrasonic() {
  digitalWrite(trigPin, LOW);  // Đảm bảo Trigger ở mức thấp trước khi phát tín hiệu
  delayMicroseconds(2);  // Đảm bảo không có tín hiệu dư

  digitalWrite(trigPin, HIGH);  // Gửi tín hiệu siêu âm
  delayMicroseconds(10);  // Kéo dài tín hiệu gửi (10 microseconds)
  digitalWrite(trigPin, LOW);  // Dừng tín hiệu

  duration = pulseIn(echoPin, HIGH);  // Đo thời gian phản hồi, tối đa 30ms (điều chỉnh thời gian chờ nếu cần)
  distance = duration * 0.034 / 2;    // Tính khoảng cách từ thời gian phản hồi


  // Kiểm tra nếu khoảng cách không hợp lệ hoặc ngoài phạm vi
  if (distance <= 0 || distance > 400) {
    distance = -1;  // Đặt giá trị lỗi
    Serial.println("No valid echo response or out of range");
  } else {
    Serial.print("Distance: ");
    Serial.println(distance);  // In ra khoảng cách
  }
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

void stopMotors(){
    digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

