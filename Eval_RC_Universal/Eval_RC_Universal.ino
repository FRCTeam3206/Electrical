#include <Servo.h>

Servo frontleftMotor;
Servo frontrightMotor;
Servo rearleftMotor;
Servo rearrightMotor;

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
#define STEERING_PIN      8  // steering channel from RC receiver
#define THROTTLE_PIN      9  // throttle channel from RC receiver
// #define AUX_PIN          12  // aux channel from RC receiver

// PWM pins: 3, 5, 6, 9, 10, and 11 (output)
#define FL_MOTOR_PIN      9  // Front Left
#define FR_MOTOR_PIN     10  
#define RL_MOTOR_PIN     11  
#define RR_MOTOR_PIN     12 


// RSL pins
#define RSL1_PIN   7  
#define RSL2_PIN   12  

// Constants
#define PULSE_WIDTH_DEADBAND  40      // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define K_STEERING            0.50    // speed multiplier to decrease steering sensitivity.  0.65 gives about a 4" turn radius at max throttle
#define K_SPEED               8.0    // Useful range 2 < 32.  16 is sluggish.  no tipping with 8.0  const accel multiplier to decrease max accel in a different way. 

const int MAX_SPEED = 500;        // This corresponds to a range of 1000 to 2000
const bool debugOn = false;       // The overhead required by serial and delays would be bad for RC control. 
                                  // Make it easy to disable.

const bool RightReverse = true;  // Flips motor cw to ccw.
const bool LeftReverse = false;  // Flips motor cw to ccw.

// Variables
long accelLimitedThrottle;        // Range will be +/- 500.  No real value to give it an initial value?

int left_speed;
int right_speed;
int l_speed; 
int r_speed;

void setup()                        
{
  if (debugOn) Serial.begin(9600);    // initialize serial communication at 9600 bits per second

  frontleftMotor.attach(FL_MOTOR_PIN); 
  frontrightMotor.attach(FR_MOTOR_PIN);               
  rearleftMotor.attach(RL_MOTOR_PIN); 
  rearrightMotor.attach(RR_MOTOR_PIN);               
//  auxMotor.attach(AUX_MOTOR_PIN);                   
}

void loop()
{   
/*  
 *   RC input
 *   Is there lag with pulseIn without a signal?
 */
 
//  int throttle = pulseIn(THROTTLE_PIN, HIGH);  // Read throttle input.  Normally 1000 to 2000
//  int steering = pulseIn(STEERING_PIN, HIGH);  // Read steering input.  Normally 1000 to 2000
//  int aux = pulseIn(AUX_PIN, HIGH);            // Read aux input.  Either ~1000 or ~2000 (basically binary)
  int right_speed = pulseIn(RIGHT_Y_AXIS_PIN, HIGH);  // Read throttle input.  Normally 1000 to 2000
  int left_speed = pulseIn(LEFT_Y_AXIS_PIN, HIGH);  // Read steering input.  Normally 1000 to 2000

//  accelLimitedThrottle = (accelLimitedThrottle/K_SPEED*(K_SPEED-1.0)) + throttle/K_SPEED;   

/*  
 *   Print RC pulse widths to Serial Monitor
 */
/*  if (debugOn) {
    Serial.print("Throttle = ");
    Serial.print(throttle);
    Serial.print(", Steering = ");
    Serial.println(steering);
//    Serial.print(", Aux = ");
//    Serial.println(aux);
    delay(1000);        // delay between reads for stability; only necessary with serial
  }
*/

  if (debugOn) {
    Serial.print("right_in = ");
    Serial.print(right_speed);
    Serial.print(", left_in = ");
    Serial.println(left_speed);
//    Serial.print(", Aux = ");
//    Serial.println(aux);
    delay(1000);        // delay between reads for stability; only necessary with serial
  }
/*
 * Important! The sw confirms communication to the controller before turning on any motors.
 */

/*  if ((900 < throttle)&& (throttle < 2100) && (900 < steering) && (steering < 2100)){
     if (debugOn) Serial.println("Yay!");

    
     // Normalize throttle and steering to the range of -500 to +500.
     long throttleNorm = throttle - 1500;
     long steeringNorm = steering - 1500;
    if (debugOn) Serial.println(steeringNorm);    
   
     steeringNorm = steeringNorm * K_STEERING;                                 // Apply simple scaling to keep steering reasonable
     accelLimitedThrottle = (accelLimitedThrottle/K_SPEED*(K_SPEED-1.0)) + throttleNorm/K_SPEED;   
    
    if (debugOn) Serial.println(K_SPEED);    
    if (debugOn) Serial.println(steeringNorm);    
    if (debugOn) Serial.println(K_STEERING);    
 
     // Mix throttle and steering inputs to obtain left & right motor speeds
     left_speed = accelLimitedThrottle - steeringNorm;
     right_speed = accelLimitedThrottle + steeringNorm;

   End of case structure.
   Do tank outputs now.
*/

  if ((900 < left_speed) && (left_speed < 2100) && (900 < right_speed) && (right_speed < 2100)){
     left_speed -= 1500; 
     right_speed -= 1500; 

         if (debugOn) {
      Serial.print("left_speed = ");
      Serial.print(l_speed);
      Serial.print(", right_speed = ");
      Serial.println(r_speed);     
    }

     // Cap speeds to max
     left_speed = min(max(left_speed, -MAX_SPEED), MAX_SPEED);  // max is 500
     right_speed = min(max(right_speed, -MAX_SPEED), MAX_SPEED); // max is 500

     // All good.
     // Values are now +/- 500 and need to be mapped to "servo" style outputs for motor controller
  }
  else     // At least one RC signal is not good
   {
    if (debugOn) Serial.println("Sad face");    
    left_speed = 0;
    right_speed = 0;
   }

    if (abs(left_speed) > PULSE_WIDTH_DEADBAND){
       if (LeftReverse) {
         left_speed = -left_speed; 
       }
       l_speed = map(left_speed, -500, 500, 1000, 2000); 
    }
    else{
       l_speed = 1500; // This should be neutral, no speed.  If it is in the deadband then call it neutral.
    }

    if (abs(right_speed) > PULSE_WIDTH_DEADBAND){
       if (RightReverse) {
         right_speed = -right_speed; 
       }
         r_speed = map(right_speed, -500, 500, 1000, 2000); 
    }
    else{
       r_speed = 1500; // This should be neutral, no speed
    }

    if (debugOn) {
      Serial.print("left_speed = ");
      Serial.print(l_speed);
      Serial.print(", right_speed = ");
      Serial.println(r_speed);     
    }

    frontleftMotor.writeMicroseconds(l_speed);       
    frontrightMotor.writeMicroseconds(r_speed);       
    rearleftMotor.writeMicroseconds(l_speed);       
    rearrightMotor.writeMicroseconds(r_speed);       
}
