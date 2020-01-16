#include <Servo.h>

#define buttonLED 4 // led on the button
#define button 5 // button acting as on/off 
#define trigPin 6 // trig pin on the ultrasoon
#define echoPin 7 // echo pin on the ultrasoon

Servo leftServo;
Servo rightServo;
int buttonState = LOW;
int previousState;
bool buttonSwitch = false;
const int rightAngleMin = 0;
const int rightAngleMax = 180;
const int leftAngleMin = 0;
const int leftAngleMax = 180;
const int distanceMax = 50;

long distance = 0;
unsigned long timeOut = 5000;
long pulseTime;

void setup() {
  Serial.begin(9600);
  leftServo.attach(9);
  rightServo.attach(10);
  leftServo.write(leftAngleMin);
  rightServo.write(rightAngleMin);
  pinMode(button, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);


}

void loop() {
  while ( buttonSwitch == true) {
    getDistance();
    if ( distance > 50) {
      distance = 50;
    }
    if (distance != 0) {
      float motorPosition = map(distance, 10 , 50, leftAngleMax, leftAngleMin);
      Serial.println(motorPosition);
      if (distance > 10 && distance != 0) {
        if (motorPosition > leftAngleMax) {
          motorPosition = leftAngleMax;
        }
        leftServo.write(motorPosition);
        rightServo.write(motorPosition);
      } else {
        leftServo.write(leftAngleMax);
        leftServo.write(leftAngleMin);
        rightServo.write(rightAngleMax);
        rightServo.write(rightAngleMax);
      }
    }
  }
}
  void buttonCheck() {
    buttonState = digitalRead(button);
    if (buttonState != previousState) {
      previousState = buttonState;
    }
    if (buttonState == HIGH) {
      buttonSwitch = !buttonSwitch;
    }
    if ( buttonSwitch == true) {
      digitalWrite(4, HIGH);
    } else {
      digitalWrite(4, LOW);
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
