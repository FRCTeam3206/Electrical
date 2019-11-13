#include <Servo.h>

Servo leftFrontMotor;
Servo rightFrontMotor;
Servo leftRearMotor;
Servo rightRearMotor;
Servo intakeMotor;

//Pin Definitions

#define ch1      7  // RC controller 1, channel (ch1) to Arduino input pin (7)
#define ch2      2  
#define ch3      3  
#define ch4      4  
#define ch5      5  
#define ch6      6  

#define ch1      8  // RC controller 2, channel (ch2) to Arduino input pin (8)  *** Bug ***

#define LEFT_FRONT_MOTOR_PIN    9  // confirmed pin  9 is pwm output
#define RIGHT_FRONT_MOTOR_PIN  10  // confirmed pin 10 is pwm output
#define LEFT_REAR_MOTOR_PIN    11  // confirmed pin 11 is pwm output
#define RIGHT_REAR_MOTOR_PIN   12  // this pin is not PWM output; to do: confirm functionality 
//#define INTAKE_MOTOR_PIN       13  // this pin is not PWM output; to do: confirm functionality 

// Constants
#define PULSE_WIDTH_DEADBAND    25    // Pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
const int MAX_SPEED = 500;            // This corresponds to a range of 1000 to 2000
const bool debugOn = false;           // The overhead required by serial and delays would be bad for RC control. Easy to disable.
const int NEUTRAL    = 1500;

void setup()                        
{
  if (debugOn) Serial.begin(9600);    // initialize serial communication at 9600 bits per second
  leftFrontMotor.attach(LEFT_FRONT_MOTOR_PIN, 1000, 2000);  // 1000 = min value, 2000 = max value
  rightFrontMotor.attach(RIGHT_FRONT_MOTOR_PIN, 1000, 2000);               
  leftRearMotor.attach(LEFT_REAR_MOTOR_PIN, 1000, 2000); 
  rightRearMotor.attach(RIGHT_REAR_MOTOR_PIN, 1000, 2000);               

//  intakeMotor.attach(INTAKE_MOTOR_PIN, 1000, 2000);               

// Perform a quick motor check.  This setup is prone to wires falling out.  This will spin each
// gearbox briefly.
    leftFrontMotor.writeMicroseconds(1650);       
      delay(200);
    leftFrontMotor.writeMicroseconds(1500);       
      delay(500);
    rightFrontMotor.writeMicroseconds(1350);       
      delay(200);
    rightFrontMotor.writeMicroseconds(1500);       
      delay(500);
    leftRearMotor.writeMicroseconds(1650);       
      delay(200);
    leftRearMotor.writeMicroseconds(1500);       
      delay(500);
    rightRearMotor.writeMicroseconds(1350);       
      delay(200);
    rightRearMotor.writeMicroseconds(1500);       
}

void loop()
{   
/*  RC input                       
 */
  int RightX    = pulseIn(7, HIGH);       // Robot spin
 // int RightY    = pulseIn(2, HIGH); 
  int LeftY     = pulseIn(3, HIGH);       // Robot Y
  int LeftX     = pulseIn(4, HIGH);       // Robot X
  int RightKnob = pulseIn(5, HIGH);       // K_Rotate
  int LeftKnob  = pulseIn(6, HIGH);       // K_Strafe

//  int intakeRaw = pulseIn(8, HIGH);         // Intake, RC Controller 2

/*  Important! The sw confirms communication to the controller before turning on any motors.
 *  If any input is bogus then stop motors. 
 *  This check isn't critical for "weapons" controller 2.
 */

  if (( 900 < LeftX ) && 
      ( LeftX < 2100 ) && 
      ( 900 < LeftY ) && 
      ( LeftY < 2100 ) && 
      ( 900 < RightX ) && 
      ( RightX < 2100 ) // && 
  //    ( 900 < intakeRaw ) && 
  //    ( intakeRaw < 2100 )
       ){

//  Print RC pulse widths to Serial Monitor 
  if (debugOn) {
    Serial.print("Left Y = ");
    Serial.print(LeftY);
    Serial.print(", Left X = ");
    Serial.print(LeftX);
    Serial.print(", Right X = ");
    Serial.println(RightX);
//    Serial.print(", Right Y = ");
//    Serial.print(RightY);
    Serial.print(", Aux Knob Right = ");
    Serial.print(RightKnob);
    Serial.print(", Aux Knob Left = ");
    Serial.print(LeftKnob);
//    Serial.print(", IntakeRaw = ");
//    Serial.println(intakeRaw);
    delay(1000);                        // delay between writes to serial monitor
  }

                                                          // Use LeftKnob to map % power for drive. 20% to 100%.
                                                          // Formerly was limited to 80% but is now sluggish with extra weight.
    long K_Drive_int = map(LeftKnob, 950, 1950, 20, 100); // The "map" function only returns long. 
                                                          // Hopefully LeftKnob is in-bounds.
    float K_Drive_float = K_Drive_int / 100.0; 
    float K_Strafe = min(1.2 * K_Drive_float, 1.0); 
    float drive   = K_Drive_float * (LeftY - NEUTRAL);  
    float strafe  = K_Strafe * (LeftX - NEUTRAL);

    long  K_Rotate_int = map(RightKnob, 950, 1950, 20, 70);  // Use RightKnob to map K_Rotate 20% to 60%.  Formerly was 60%.
    float K_Rotate_float = K_Rotate_int / 100.0; 
    float rotate  = K_Rotate_float * (RightX - NEUTRAL);
    
    float front_left  = drive + strafe  + rotate;
    float front_right = drive - strafe  - rotate;
    float rear_left   = drive - strafe  + rotate;
    float rear_right  = drive + strafe  - rotate;
  //  float intake      = (intakeRaw - NEUTRAL) * 1.6; // Normalized 

    // Cap speeds to max, +/- 500
     front_left  = min(max(front_left,  -MAX_SPEED), MAX_SPEED);
     front_right = min(max(front_right, -MAX_SPEED), MAX_SPEED);  
     rear_left   = min(max(rear_left,   -MAX_SPEED), MAX_SPEED); 
     rear_right  = min(max(rear_right,  -MAX_SPEED), MAX_SPEED); 
//     intake      = min(max(intake,      -MAX_SPEED), MAX_SPEED); 

  if (debugOn) {
    // Serial.print("Drive = ");
    // Serial.print(drive);
    // Serial.print(", Strafe = ");
    // Serial.print(strafe);
    // Serial.print(", K_Drive = ");
    // Serial.print(K_Drive_float);
//    Serial.print(", Intake = ");
//    Serial.println(intake);

/*    Serial.print("front_left = ");
    Serial.print(front_left);
    Serial.print(", rear_left= ");
    Serial.print(rear_left);
    Serial.print(", front_right = ");
    Serial.print(front_right);
    Serial.print(", rear_right = ");
    Serial.println(rear_right);
*/    delay(500);  //Only if debugOn
  }
     /*  Localize  to  motors
     *  It isn't entirely necessary to put bounds on the output because servo.attach has min, max arguments
     *  Something is operating very slowly in the loop.  Try updating just one of the outputs per cycle.
     */

        if (abs(front_left) < PULSE_WIDTH_DEADBAND) front_left = 0;
        leftFrontMotor.writeMicroseconds(front_left + NEUTRAL);

        if (abs(front_right) < PULSE_WIDTH_DEADBAND) front_right = 0;
        rightFrontMotor.writeMicroseconds(NEUTRAL - front_right);

        if (abs(rear_left) < PULSE_WIDTH_DEADBAND) rear_left = 0;   
        leftRearMotor.writeMicroseconds(rear_left + NEUTRAL);

        if (abs(rear_right) < PULSE_WIDTH_DEADBAND) rear_right = 0;
        rightRearMotor.writeMicroseconds(NEUTRAL - rear_right);

//        intake = NEUTRAL + intake;
        
//        if (debugOn) {
//          Serial.print(", IntakeFinal = ");
//          Serial.println(intake);
//        }

//        if (abs(intake) < PULSE_WIDTH_DEADBAND) intake = 0;
//        intakeMotor.writeMicroseconds(intake);
  }
  else {
    leftFrontMotor.writeMicroseconds(NEUTRAL);
    rightFrontMotor.writeMicroseconds(NEUTRAL);
    leftRearMotor.writeMicroseconds(NEUTRAL);
    rightRearMotor.writeMicroseconds(NEUTRAL);
//    intakeMotor.writeMicroseconds(NEUTRAL);
    }
}
