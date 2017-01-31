#include <Servo.h>

// Speed Control
//Servo myPotServo;  // create servo object to control a servo
const int potpin = A4;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int sensorValue;
int oldValue = 1500;

// Button Control
Servo myServo;
const int motorPin = 3;
const int buttonPin = 7;
int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(motorPin, 1000, 2000);
  pinMode(buttonPin, INPUT);
//  Serial.begin(9600);
}

void loop() {
  // Speed Control
  sensorValue = analogRead(potpin);

  // Switch Control
  if (digitalRead(buttonPin) == HIGH)
    {
      myServo.writeMicroseconds(1500);
//      Serial.print("HIGH\n");
      oldValue = 1500;                  // sets oldValue to its default value (1500=off)
    }
    else
    {
      if (sensorValue != oldValue) {
        sensorValue = (sensorValue + 7 * oldValue) / 8;  // filter the value to avoid jumps if noisy pot
        val = map(sensorValue, 1023, 0, 1500, 2000);     // scale it to use it with the servo (value between 0 and 180)
        myServo.writeMicroseconds(val);                  // sets the servo position according to the scaled value
        oldValue = sensorValue;  
      }      
      delay(15);
    }
}
