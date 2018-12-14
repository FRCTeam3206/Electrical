#include <Servo.h>

Servo leftFrontMotor;
Servo rightFrontMotor;
Servo leftRearMotor;
Servo rightRearMotor;

//Pin Definitions
#define ch1      7  // RC channel (ch1) to Arduino input pin (7)
#define ch2      2  
#define ch3      3  
#define ch4      4  
#define ch5      5  
#define ch6      6  

#define LEFT_FRONT_MOTOR_PIN    9  // confirmed pin  9 is pwm output
#define RIGHT_FRONT_MOTOR_PIN  10  // confirmed pin 10 is pwm output
#define LEFT_REAR_MOTOR_PIN    11  // confirmed pin 11 is pwm output
#define RIGHT_REAR_MOTOR_PIN   12  // this pin is not PWM output; to do: confirm functionality 

// Constants
#define PULSE_WIDTH_DEADBAND    40      // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)

const int MAX_SPEED = 500;            // This corresponds to a range of 1000 to 2000
const bool debugOn = false;            // The overhead required by serial and delays would be bad for RC control. 
const int NEUTRAL = 1500;                                      // Make it easy to disable.
const float K_Rotate = 0.5;
const float K_Drive = 0.75;
const float K_Strafe = 0.75;

// Variables
//long accelLimitedThrottle;        // Range will be +/- 500.  No real value to give it an initial value?

int left_speed;
int right_speed;
int l_speed; 
int r_speed;

void setup()                        
{
//  if (debugOn) Serial.begin(9600);    // initialize serial communication at 9600 bits per second

  leftFrontMotor.attach(LEFT_FRONT_MOTOR_PIN); 
  rightFrontMotor.attach(RIGHT_FRONT_MOTOR_PIN);               
  leftRearMotor.attach(LEFT_REAR_MOTOR_PIN); 
  rightRearMotor.attach(RIGHT_REAR_MOTOR_PIN);               
}

void loop()
{   
/*  
 *   RC input
 */
  int RightX    = pulseIn(7, HIGH);      // Plan to use this for Robot spin
 // int RightY    = pulseIn(2, HIGH); 
  int LeftY     = pulseIn(3, HIGH);       // Plan to use this for Robot X
  int LeftX     = pulseIn(4, HIGH);       // Plan to use this for Robot Y
//  int RightKnob = pulseIn(5, HIGH);  
//  int LeftKnob  = pulseIn(6, HIGH);  

  if ((900 < LeftX)&& (LeftX < 2100) && (900 < LeftY) && (LeftY < 2100) && (900 < RightX) && (RightX < 2100)){

/*  
 *   Print RC pulse widths to Serial Monitor
 */
 
  if (debugOn) {
    Serial.print("Left Y = ");
    Serial.print(LeftY);
    Serial.print(", Left X = ");
    Serial.print(LeftX);
    Serial.print(", Right X = ");
    Serial.println(RightX);
//    Serial.print(", Right Y = ");
//    Serial.print(RightY);
//    Serial.print(", Aux Knob Right = ");
//    Serial.print(RightKnob);
//    Serial.print(", Aux Knob Left = ");
//    Serial.println(LeftKnob);
//    delay(1000);                        // delay between writes to serial monitor
  }

/*
 * Important! The sw confirms communication to the controller before turning on any motors.
 */

/*    leftFrontMotor.writeMicroseconds(1600);       
    delay(1000);
    leftFrontMotor.writeMicroseconds(1500);       
    delay(1000);
    rightFrontMotor.writeMicroseconds(1400);       
    delay(1000);
    rightFrontMotor.writeMicroseconds(1500);       
    delay(1000);
    leftRearMotor.writeMicroseconds(1600);       
    delay(1000);
    leftRearMotor.writeMicroseconds(1500);       
    delay(1000);
    rightRearMotor.writeMicroseconds(1400);       
    delay(1000);
    rightRearMotor.writeMicroseconds(1500);       
    delay(2000000);
*/
    float drive   = LeftY - NEUTRAL;
    float strafe  = LeftX - NEUTRAL;
    float rotate  = RightX - NEUTRAL;

    rotate = K_Rotate * rotate;
    strafe = K_Strafe * strafe;
    drive  = K_Drive * drive;
    
    float  front_left  = drive + strafe  + rotate;
    float  rear_left   = drive - strafe  + rotate;
    float  front_right = drive - strafe  - rotate;
    float  rear_right  = drive + strafe  - rotate;

/*
    Serial.print("Drive = ");
    Serial.print(drive);
    Serial.print(", Strafe = ");
    Serial.print(strafe);
    Serial.print(", Rotate = ");
    Serial.println(rotate);
*/
  if (debugOn) {
    Serial.print("front_left = ");
    Serial.print(front_left);
    Serial.print(", rear_left= ");
    Serial.print(rear_left);
    Serial.print(", front_right = ");
    Serial.print(front_right);
    Serial.print(", rear_right = ");
    Serial.println(rear_right);
 //   delay(500);
  }
    //Localize  to  motor

    if (abs(front_left)> PULSE_WIDTH_DEADBAND) leftFrontMotor.writeMicroseconds(front_left + NEUTRAL);
    if (abs(rear_left)> PULSE_WIDTH_DEADBAND) leftRearMotor.writeMicroseconds(rear_left + NEUTRAL);
    if (abs(front_right)> PULSE_WIDTH_DEADBAND) rightFrontMotor.writeMicroseconds(NEUTRAL - front_right);
    if (abs(rear_right)> PULSE_WIDTH_DEADBAND) rightRearMotor.writeMicroseconds(NEUTRAL - rear_right);
  }
  else {
    leftFrontMotor.writeMicroseconds(NEUTRAL);
    leftRearMotor.writeMicroseconds(NEUTRAL);
    rightFrontMotor.writeMicroseconds(NEUTRAL);
    rightRearMotor.writeMicroseconds(NEUTRAL);
    }
}
