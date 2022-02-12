#include <Servo.h>

Servo climbMotor;
//Servo frontrightMotor;
//Servo rearleftMotor;
//Servo rearrightMotor;

/*
 * Purpose: create a ~universal arduino code for bot control.
 *        Four states, 
 *        0 = no controllers -> motors off 
 *        1 = Dual Joystick, tank drive
 *        2 = Pistol Grip    
 *        3 = Both controllers, Pistol Grip is Master
 *        
 *        Use a case structure and two RSL to indicate states.
 *             
 * 
 */

/*
 * To do: add hardware 2x RSL with relay.  Add RSL sw.
 * 
 */

//Pin Definitions; DIO pins 0 and 1 are mainly for serial comms, don't use them.

//Dual Joystick Controller (input)
// #define ch1            7  // Right X-Axis. RC channel (ch1) to Arduino input pin (7); 
#define RIGHT_Y_AXIS_PIN  2  // Right Y-Axis
#define LEFT_Y_AXIS_PIN   3  // Left Y-Axis
// #define ch4            4  // Left X-Axis
// #define ch5            5  // RightKnob
// #define ch6            6  // LeftKnob

//Pistol Grip (input)
//#define STEERING_PIN      8  // steering channel from RC receiver
//#define THROTTLE_PIN      9  // throttle channel from RC receiver
// #define AUX_PIN          12  // aux channel from RC receiver

// PWM pins: 3, 5, 6, 9, 10, and 11 (output)
#define Climb_PIN      10  // Motor output pin
//#define FR_MOTOR_PIN     10  
//#define RL_MOTOR_PIN     11  
//#define RR_MOTOR_PIN     12 


// RSL pins
#define RSL1_PIN   7  
#define RSL2_PIN   12  

// Constants
#define PULSE_WIDTH_DEADBAND  40      // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)

// This Max of 390 is here because the joystick is only outputting a range of about +/-400 and wouldn't otherwise utilize the full output range of the motor.
const int MAX_SPEED = 390;        // This corresponds to a range of 1000 to 2000

const bool debugOn = false;       // The overhead required by serial and delays would be bad for RC control. 
                                  // Make it easy to disable.



int climb_speed;
int c_speed; 

void setup()                        
{
  if (debugOn) Serial.begin(9600);    // initialize serial communication at 9600 bits per second

  climbMotor.attach(Climb_PIN); 
}

void loop()
{   
/*  
 *   RC input
 *   Is there lag with pulseIn without a signal?
 */
 
  int climb_speed = pulseIn(RIGHT_Y_AXIS_PIN, HIGH);  // Read throttle input.  Normally 1000 to 2000


/*  
 *   Print RC pulse widths to Serial Monitor
 */

  if (debugOn) {
    Serial.print("Controller climb_in = ");
    Serial.println(climb_speed);
    delay(1000);        // delay between reads for stability; only necessary with serial
  }
/*
 * Important! The sw confirms communication to the controller before turning on any motors.
 */


  if ((900 < climb_speed) && (climb_speed < 2100)) { // && (900 < right_speed) && (right_speed < 2100)){
     climb_speed -= 1500; 

     // Cap speeds to max
     climb_speed = min(max(climb_speed, -MAX_SPEED), MAX_SPEED);  // max is 500
//     right_speed = min(max(right_speed, -MAX_SPEED), MAX_SPEED); // max is 500

     // All good.
     // Values are now +/- 500 and need to be mapped to "servo" style outputs for motor controller
  }
  else     // At least one RC signal is not good
   {
    if (debugOn) Serial.println("Sad face");    
    climb_speed = 0;
//    right_speed = 0;
   }

    if (abs(climb_speed) > PULSE_WIDTH_DEADBAND){
//       if (LeftReverse) {
//         left_speed = -left_speed; 
//       }
       c_speed = map(climb_speed, -390, 390, 1000, 2000); 
    }
    else{
       c_speed = 1500; // This should be neutral, no speed.  If it is in the deadband then call it neutral.
    }

    if (debugOn) {
      Serial.print("Motor c_speed out = ");
      Serial.println(c_speed);
    }

    climbMotor.writeMicroseconds(c_speed);       
}
