#include <Servo.h>

Servo leftMotor;
Servo rightMotor;
Servo auxMotor;

//Pin Definitions
#define STEERING_PIN      2  // steering channel from RC receiver
#define THROTTLE_PIN      4  // throttle channel from RC receiver
#define AUX_PIN           5  // aux channel from RC receiver
#define LEFT_MOTOR_PIN    9  // confirmed pin  9 is pwm output
#define RIGHT_MOTOR_PIN  10  // confirmed pin 10 is pwm output
// #define AUX_MOTOR_PIN    11  // confirmed pin 11 is pwm output

// Constants
#define PULSE_WIDTH_DEADBAND  40      // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define K_STEERING            0.50    // speed multiplier to decrease steering sensitivity.  0.65 gives about a 4" turn radius at max throttle
 

int MAX_SPEED = 500;        // This corresponds to a range of 1000 to 2000
const bool debugOn = false;       // The overhead required by serial and delays would be bad for RC control. 
                                  // Make it easy to disable.

const bool RightReverse = true;  // Flips motor cw to ccw.
const bool LeftReverse = false;  // Flips motor cw to ccw.

// Variables
long accelLimitedThrottle;        // Range will be +/- 500.  No real value to give it an initial value?
float K_ACCEL = 8.0;   // Useful range 2 < 32.  16 is sluggish.  no tipping with 8.0  const accel multiplier to decrease max accel in a different way.
float K_SPEED = 0.5;  // Multiplier for throttle input

bool RSL = true;
unsigned long t_last;
unsigned long t_elapsed;
unsigned long d_blink = 1000.0;

char stateRobot = 'A';

int left_speed;
int right_speed;
int l_speed; 
int r_speed;
int NEUTRAL = 1500;

void setup()                        
{
  if (debugOn) Serial.begin(9600);    // initialize serial communication at 9600 bits per second

  leftMotor.attach(LEFT_MOTOR_PIN); 
  rightMotor.attach(RIGHT_MOTOR_PIN);               
//  auxMotor.attach(AUX_MOTOR_PIN);
  pinMode(LED_BUILTIN, OUTPUT);  // pin 13 on the arduino    
  t_last = millis();               
}

void loop()
{   
/*  
 *   RC input
 */
  int throttle = pulseIn(THROTTLE_PIN, HIGH);  // Read throttle input.  Normally 1000 to 2000
  int steering = pulseIn(STEERING_PIN, HIGH);  // Read steering input.  Normally 1000 to 2000
  int aux = pulseIn(AUX_PIN, HIGH);            // Read aux input.  Either ~1000 or ~2000 (basically binary)

  t_elapsed = millis() - t_last;

  if (t_elapsed > d_blink) {
    RSL = !RSL;
    t_elapsed = 0;
    t_last = millis();
  }

  if (RSL) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
  
//  accelLimitedThrottle = (accelLimitedThrottle/K_SPEED*(K_SPEED-1.0)) + throttle/K_SPEED;   
 
/*  stateRobot controls robot through autonomous states, last state is driver controlled
 *   
 *   To Do:  improve responsiveness by replacing delay statements
 */
  
  switch (stateRobot) {
    case 'A':                           //motor check
        if (throttle > 1800) {
          driveRobot (0.2,0.2,250);
          neutralRobot();
          delay(2000);
          stateRobot = 'B';
        }
        else {
          stateRobot = 'Z';
        }
        break;
    case 'B':                         //forward 10ish ft
        driveRobot (0.2,0.2,3500);   //left speed (range of (-1,1) ), right speed (range of (-1,1) ), duration
        neutralRobot();
        delay(2000);
        stateRobot = 'C';
        break;  
    case 'C':                         //backward 10ish ft
        driveRobot (-0.2,-0.2,3500);
        neutralRobot();
        delay(5000);
        stateRobot = 'D';
        break;
    case 'D':                          //turn left 90 degrees ish
        driveRobot (0.3,-0.2,2200);
        neutralRobot();
        delay(5000);
        stateRobot = 'E';
        break;
    case 'E':                         //backward 10ish ft
        driveRobot (-0.2,-0.2,3500);
        neutralRobot();
        delay(5000);
        stateRobot = 'Y';
        break;
    case 'Y':
        //do something here
        stateRobot = 'Z';
        break;
    default: 
    //do something here
  
/*  
 *   Print RC pulse widths to Serial Monitor
 */
  if (debugOn) {
    Serial.print("Throttle = ");
    Serial.print(throttle);
    Serial.print(", Steering = ");
    Serial.print(steering);
    Serial.print(", Aux = ");
    Serial.println(aux);
    delay(1000);        // delay between reads for stability; only necessary with serial
  }

/*
 * Important! The sw confirms communication to the controller before turning on any motors.
 */
 
  if ((900 < throttle)&& (throttle < 2100) && (900 < steering) && (steering < 2100)){
     if (debugOn) Serial.println("Yay!");

    
     // Normalize throttle and steering to the range of -500 to +500.
     long throttleNorm = (throttle - 1500) * K_SPEED ;
     long steeringNorm = steering - 1500;
    if (debugOn) Serial.println(K_SPEED);    
    
    if (aux > 1500) {
      K_ACCEL = 1.0;
      K_SPEED = 1.0;
      d_blink = 7.0;  //blink real fast, rave 
    }
    else { 
      K_ACCEL = 8.0;
      K_SPEED = 0.7;
      d_blink = 350.0; //blink normal speed
    }
     steeringNorm = steeringNorm * K_STEERING;                                 // Apply simple scaling to keep steering reasonable
     accelLimitedThrottle = (accelLimitedThrottle/K_ACCEL*(K_ACCEL-1.0)) + throttleNorm/K_ACCEL;   
    
  //  if (debugOn) Serial.println(K_SPEED);    
    if (debugOn) Serial.print("something else!");
 if (debugOn) Serial.println(throttleNorm);    
  //  if (debugOn) Serial.println(K_STEERING);    
 

     // Mix throttle and steering inputs to obtain left & right motor speeds
     left_speed = accelLimitedThrottle - steeringNorm;
     right_speed = accelLimitedThrottle + steeringNorm;

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

    leftMotor.writeMicroseconds(l_speed);       
    rightMotor.writeMicroseconds(r_speed);  
      break;
  } //last of the case statement for driver control     
}

void driveRobot(float L, float R, float t) {
        leftMotor.writeMicroseconds(-500 * L + 1500);       //sets motor speed range (-1,1) for both motors for duration t
        rightMotor.writeMicroseconds(500 * R + 1500);       //does not turn motors off
        delay(t);
}

void neutralRobot() {
        leftMotor.writeMicroseconds(NEUTRAL);       //sets speed of both drive motors to zero
        rightMotor.writeMicroseconds(NEUTRAL);
}
