#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A4;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int sensorValue;
int oldValue = 1500;

void setup() {
  myservo.attach(3, 1000, 2000);  // attaches the servo on pin 3 to the servo object
//  Serial.begin(9600);
  }

void loop() {
//  val = analogRead(A4);            // reads the value of the potentiometer (value between 0 and 1023)
  sensorValue = analogRead(potpin);
/* Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
*  float voltage = sensorValue * (5.0 / 1023.0);
*  print out the value you read:
*  Serial.println(voltage);
*/

//to do, if pot didn't change then don't write
  if (sensorValue != oldValue) {
    sensorValue = (sensorValue + 7 * oldValue) / 8;  // filter the value to avoid jumps if noisy pot
    val = map(sensorValue, 1023, 0, 1500, 2000);     // scale it to use it with the servo (value between 0 and 180)
    myservo.writeMicroseconds(val);                  // sets the servo position according to the scaled value
    oldValue = sensorValue;
  }
  delay(15);
                             
}

