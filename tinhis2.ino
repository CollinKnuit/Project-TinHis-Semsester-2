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
const int rightAngleMax = 75;
const int leftAngleMin = 179;
const int leftAngleMax = 105;
const int distanceMax = 50;

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


}

void loop() {
  while ( buttonSwitch == true) {
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
