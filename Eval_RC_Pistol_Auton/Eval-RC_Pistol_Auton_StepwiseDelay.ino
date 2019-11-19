#include <Servo.h>

Servo leftMotor;
Servo rightMotor;
Servo auxMotor;

//Pin Definitions
#define STEERING_PIN      2         // steering channel from RC receiver
#define THROTTLE_PIN      4         // throttle channel from RC receiver
#define AUX_PIN           5         // aux channel from RC receiver
#define LEFT_MOTOR_PIN    9         // confirmed pin  9 is pwm output
#define RIGHT_MOTOR_PIN  10         // confirmed pin 10 is pwm output
//#define AUX_MOTOR_PIN    11         // confirmed pin 11 is pwm output

// Constants
#define PULSE_WIDTH_DEADBAND  40    // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define K_STEERING            0.50  // speed multiplier to decrease steering sensitivity.  0.65 gives about a 4" turn radius at max throttle


int MAX_SPEED = 500;                // This corresponds to a range of 1000 to 2000
const bool debugOn = false;         // The overhead required by serial and delays would be bad for RC control.
                                    // Make it easy to disable.

const bool RightReverse = true;     // Flips motor cw to ccw.
const bool LeftReverse = false;     // Flips motor cw to ccw.

// Variables
long accelLimitedThrottle;          // Range will be +/- 500.  No real value to give it an initial value?
float K_ACCEL = 8.0;                // Useful range 2 < 32.  16 is sluggish.  no tipping with 8.0  const accel multiplier to decrease max accel in a different way.
float K_SPEED = 0.5;                // Multiplier for throttle input

bool RSL = true;
unsigned long t_last;
unsigned long t_elapsed;
unsigned long d_blink = 1000;

// STATES and CONSTANTS
#define STATE_STEP_DELAY     50                // Reduce for finer-grained timing

// Drive states - give them meaningful names
#define DRIVE_CHECK           1
#define DRIVE_FW_10FT         2
#define DRIVE_BK_10FT         3
#define DRIVE_90_LEFT         4
#define DRIVE_90_RIGHT        5
#define DRIVE_DO_SOMETHING    6                // Yeah, do something with this...
#define DRIVE_TELEOP         99                // Manual

unsigned int drive_state = DRIVE_CHECK;        // Initial Autonomous Drive State
unsigned long t_drive_elapsed = 0;             // Resets for each drive state change

// GAME TIME MAXES
#define MAX_AUTONOMOUS      30000              // 30 seconds

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

  /**
   * For each drive state that takes longer than standard step
   * definition, create a function that returns a 1 for "still
   * running" or 0 for "done". Set the next autonomous state. States
   * can do more than one thing.
   */
  switch (drive_state) {
  case DRIVE_CHECK:
    if (throttle > 1800) {
      if !(driveCheck(t_drive_elapsed)) {
          setDriveState(DRIVE_FW_10FT);
        }
    } else {
      setDriveState(DRIVE_TELEOP);
    }
    break;
  case DRIVE_FW_10FT:
    if !(driveForward10Ft(t_drive_elapsed)) {
        setDriveState(DRIVE_BK_10FT);
      }
    break;
  case DRIVE_BK_10FT:
    if !(driveBackward10Ft(t_drive_elapsed)) {
        setDriveState(DRIVE_90_LEFT);
      }
    break;
  case DRIVE_LEFT_90:
    if !(drive90Left(t_drive_elapsed)) {
        setDriveState(DRIVE_90_RIGHT);
      }
    break;
  case DRIVE_90_RIGHT:
    if !(drive90Right(t_drive_elapsed)) {
        setDriveState(DRIVE_DO_SOMETHING);
      }
    break;
  case DRIVE_DO_SOMETHING:
    //do something here
    neutralDrive();
    setDriveState(DRIVE_TELEOP);
    break;
  default:
    driveTeleOp(throttle, steering, aux);
    return(); // no delay, infinite loop
  }

  // TODO: Add Aux motor state switch here, etc.

  // In states other than teleop, delay a "standard time"
  delay(STATE_STEP_DELAY);
  t_drive_elapsed = t_drive_elapsed + STATE_STEP_DELAY;
}

// Sends signal to survos to set speed. Left and Right motors move reverse of each other.
void setMotorSpeeds(float L, float R) {
  leftMotor.writeMicroseconds(-500 * L + 1500);
  rightMotor.writeMicroseconds(500 * R + 1500);
}

// Sets speed of both drive motors to zero (doesn't "stop" the bot)
void neutralDrive() {
  leftMotor.writeMicroseconds(NEUTRAL);
  rightMotor.writeMicroseconds(NEUTRAL);
}

// Set the next drive state and reset the drive state elapsed timer
void setDriveState(unsigned long state) {
  drive_state = state;        // Next state
  t_drive_elapsed = 0;        // Reset drive elapsed timer
}


// Drive forward and backward a short distance, then sit
// Duration: at least 1.5 seconds
int driveCheck(unsigned long elapsed) {
  if (elapsed < 250) {
    setMotorSpeeds (0.2, 0.2);
    return(1);
  } else if (elapsed < 500) {
    setMotorSpeeds (-0.2, -0.2);
    return(1);
  } else if (elapsed < 750) {
    neutralDrive();
    return(1);
  }
  return(0);
}

// Drive forward 10 feet
// Duration: at least 3.5 seconds
int driveForward10Ft(unsigned long elapsed) {
  if (elapsed < 3500) {
    setMotorSpeeds (0.2, 0.2);
    return(1);
  }
  return(0);
}

// Drive forward 10 feet
// Duration: 3.5 seconds
int driveBackward10Ft(unsigned long elapsed) {
  if (elapsed < 3500) {
    setMotorSpeeds (-0.2, -0.2);
    return(1);
  }
  return(0);
}

// Turn Right 90
// Duration: 2.2 seconds
int driveRight90(unsigned long elapsed) {
  if (elapsed < 2200) {
    setMotorSpeeds (0.2, -0.3);
    return(1);
  }
  return(0);
}

// Turn Left 90
// Duration: 2.2 seconds
int driveLeft90(unsigned long elapsed) {
  if (elapsed < 2200) {
    setMotorSpeeds (-0.3, 0.2);
    return(1);
  }
  return(0);
}

// Teleop
// Duration: infinite
int driveTeleOp(int throttle, int steering, int aux) {
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

  if (abs(left_speed) > PULSE_WIDTH_DEADBAND) {
    if (LeftReverse) {
      left_speed = -left_speed;
    }
    l_speed = map(left_speed, -500, 500, 1000, 2000);
  }
  else {
    l_speed = 1500; // This should be neutral, no speed.  If it is in the deadband then call it neutral.
  }

  if (abs(right_speed) > PULSE_WIDTH_DEADBAND){
    if (RightReverse) {
      right_speed = -right_speed;
    }
    r_speed = map(right_speed, -500, 500, 1000, 2000);
  }
  else {
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
  return(1);
}
