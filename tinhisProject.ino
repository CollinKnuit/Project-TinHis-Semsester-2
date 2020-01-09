#include <Servo.h>

#define switchPin 5 // pin for the tumble switch
#define trigPin  6 // trig pin on the ultrasoon
#define echoPin  7 // echo pin on the ultrasoon

Servo switchServo;    // the switchServo sis the one whose arms put the tumble switch of
Servo ultrasoonServo; // the ultrasoonServo reachts of the ultrasoon

float distance = 0;   // variable to store the measured distance from the ultrasonic sensor
boolean tumbleState = false; //the state of the tumble switch
boolean switching = false; // used to control the state of the device and handle the delay of the servo rotation

const int switchAngleMax = 0;             //the max angle that the switch servo can extend
const int switchAngleMin = 180;            //the min angle that the switch servo can extend
const int ultrasoonAngleMax = 0;         //the max angle that the ultrasoon servo can extend
const int ultrasoonAngleMin = 180;        ///the min angle that the ultrasoon servo can extend
const int distanceMax = 50;              //the maximum distance away from the sensor where the ultrasoon servo will respond
const float threshold = 0.93; // percentage of the maximum travel of the ultrasoon servo where the servo arm should fully commit to pressing the switch
const float threshCommit = threshold * switchAngleMin; // the calculated angle when the servo arm should commit to pressing the switch

void setup() {
  Serial.begin(9600);
  switchServo.attach(9);
  ultrasoonServo.attach(10);
  switchServo.write(switchAngleMax);        //set initial position of the switch servo
  ultrasoonServo.write(ultrasoonAngleMin);  //set initial position of the ultrasoon servo
  pinMode(switchPin, INPUT_PULLUP); // a pullup resistor to not complicate the circuit
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  // if the switch was already on when the device powers on, turn the switch off before entering the main loop
  if (digitalRead(switchPin) == HIGH) { // check if the switch is turned on when it was previously off

    switching = true;
    tumbleState = true;
    switchServo.write(switchAngleMax); // set position of switch arm to hit the switch back off
    ultrasoonServo.write(ultrasoonAngleMax); // retract the ultrasoon arm while the switch arm turns the switch off

    while (switching) { // don't do anything except wait for the arm to hit the switch

      if (digitalRead(switchPin) == LOW) { // check if the tumble switch is off yet

        tumbleState = false;
        switching = false;
        switchServo.write(switchAngleMin); // retract the top arm now that the switch is off
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(switchPin) == HIGH) { // check if the switch was changed before the switch arm could do it

    switching = true;
    resetSwitch();
  }
  distance = getDistance();
  if (distance < distanceMax) { // update the motor position when its close enough

    updateUltraSoonArm();
    delay(15);
  }
  else {

    switchServo.write(switchAngleMin); // retract the switch arm
  }
}

// Restting the switch
void resetSwitch() {

  switchServo.write(switchAngleMax); // set position of switch arm to hit the switch
  while (switching == true) { // don't do anything except wait for the arm to hit the switch

    if (digitalRead(switchPin) == LOW) { // check if the switch is off yet

      tumbleState = false;
      switching = false;
      switchServo.write(switchAngleMin); // retract the switch arm now that the switch is off
    }
  }
}

// something I found online
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  float duration = pulseIn(echoPin, HIGH);
  float measuredDistance = duration/58.2;                                                                       
  return measuredDistance;
}


void updateUltraSoonArm() {
  float motorPosition = map(distance, 0, distanceMax, ultrasoonAngleMax, ultrasoonAngleMin); // using a map to change the distance into a value that can be worked with
  Serial.println(motorPosition);
  if (distance < 7) {

    commit();
  } else {

    ultrasoonServo.write(motorPosition);
  }
}

// If the ultrasoon servo is over the treshold commit to pushing the button
void commit() {
  
  ultrasoonServo.write(ultrasoonAngleMax);
  while (!tumbleState) {
  Serial.println(tumbleState);
    if (digitalRead(switchPin) == HIGH) {
  Serial.println("help");
      tumbleState = true;
      switching = true;
      ultrasoonServo.write(ultrasoonAngleMin);
    }
  }
  resetSwitch();
}
