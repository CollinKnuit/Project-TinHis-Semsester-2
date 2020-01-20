#include <Servo.h>

#define buttonLED 4 // led on the button
#define button 5 // button acting as on/off 
#define trigPin 6 // trig pin on the ultrasoon
#define echoPin 7 // echo pin on the ultrasoon
#define ON 15
#define OFF 16

Servo leftServo;
Servo rightServo;
bool buttonState = false;
bool loopnow;
bool buttonSwitch = true;
const int rightAngleMin = 0;
const int rightAngleMax = 75;
const int leftAngleMin = 179;
const int leftAngleMax = 105;
const int distanceMax = 50;
int state;

long distance = 0;
unsigned long timeOut = 5000;
long pulseTime;

void setup() {
  Serial.begin(9600);
  leftServo.attach(10);
  rightServo.attach(9);
  leftServo.write(leftAngleMin);
  rightServo.write(rightAngleMin);
  pinMode(button, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  state = OFF;


}

void loop() {
  switch (state) {
    case OFF:
      buttonCheckOFF();
      break;
    case ON:
      getDistance();
      if ( distance > 50) {
        distance = 50;
      }
      if (distance != 0) {
        float leftMotorPosition = map(distance, 10 , 50, leftAngleMax, leftAngleMin);
        float rightMotorPosition = map(distance, 10 , 50, rightAngleMax, rightAngleMin);

        Serial.println(leftMotorPosition);
        if (distance > 10 && distance != 0) {
          if (rightMotorPosition > rightAngleMax) {
            rightMotorPosition = rightAngleMax;
          }
          if (leftMotorPosition < leftAngleMax) {
            leftMotorPosition = leftAngleMax;
          }
          leftServo.write(leftMotorPosition);
          rightServo.write(rightMotorPosition);
        } else if ( distance < 10) {
          leftServo.write(leftAngleMax);
          rightServo.write(rightAngleMax);
        } else if ( distance == 0) {
          leftServo.write(leftAngleMin);
          rightServo.write(rightAngleMin);
        }
      }
      buttonCheckON();
      break;
  }
}
void buttonCheckOFF() {
  if (digitalRead(button)) {
    digitalWrite(buttonLED, HIGH);
    Serial.print("device turned ON");
    state = ON;
    delay(500);
  }
}

void buttonCheckON() {
  if (digitalRead(button)) {
    digitalWrite(buttonLED, LOW);
    Serial.print("device turned OFF");
    state = OFF;
    delay(500);
  }
}

void getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  pulseTime = pulseIn (echoPin, HIGH, timeOut);
  distance = pulseTime / 58.2;
  Serial.println(distance);
}
