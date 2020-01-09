#include <Servo.h>

#define switchPin 5 // pin for the tumble switch
#define trigPin 6 // trig pin on the ultrasoon
#define echoPin 7 // echo pin on the ultrasoon

#define TUMBLER 12
#define SONAR 11

Servo switchServo;    // the switchServo sis the one whose arms put the tumble switch of
Servo sonarServo; // the ultrasoonServo reachts of the ultrasoon

bool switcher;
const int switchAngleMin = 0;
const int switchAngleMax = 180;
const int sonarAngleMin = 0;
const int sonarAngleMax = 180;
const int distanceMax = 50;

long distance = 0;   // variable to store the measured distance from the ultrasonic sensor
unsigned long timeOut = 5000;
long pulseTime;

void setup() {
  Serial.begin(9600);
  switchServo.attach(9);
  sonarServo.attach(10);
  switchServo.write(switchAngleMin);
  sonarServo.write(sonarAngleMin);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);


  sonarServo.write(sonarAngleMin);
  switchServo.write(sonarAngleMin);
  if (digitalRead(switchPin) == LOW) {
    switcher = true;
  } else {
    switcher = false;
  }

  switcher = true;
}

void loop() {
while (switcher == true) {
  switchServo.write(switchAngleMax);
  delay(1000);
  // if (digitalRead(switchPin) == HIGH ) {
  switchServo.write(switchAngleMin);
  delay(1000);
  switcher = false;
  //   }
}
  while (switcher == false) {
    getDistance();
    if ( distance > 50) {
      distance = 50;
    }
    if (distance != 0) {
      float motorPosition = map(distance, 10 , 50, sonarAngleMax, sonarAngleMin);
      Serial.println(motorPosition);
      if (distance > 10 && distance != 0) {
        if (motorPosition > sonarAngleMax) {
          motorPosition = sonarAngleMax;
        }
        sonarServo.write(motorPosition);
      } else {
        sonarServo.write(sonarAngleMax);
        //  if (digitalRead(switchPin) == LOW) {
        sonarServo.write(sonarAngleMin);
        switcher = true;
      }
    }
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

/*
  case tumbler

  angle 0 -> angle ??? -> tumble gets tumbled -> swtich case

  case sonar

  sonar 5-50 cm get mapped to angle -> under  gets triggered -> tumble gets tumbled -> switch case
*/
