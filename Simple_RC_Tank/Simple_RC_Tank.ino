#include <Servo.h>

/*
  Minimal program to control a robot with a RC Pistol Grip
*/

/* Pins used for inputs from the RC receiver */
#define LEFT_STICK_IN      5  // left_stick channel from RC receiver
#define RIGHT_STICK_IN      4  // right_stick channel from RC receiver
#define AUX_IN           6  // aux channel from RC receiver

#define CENTER        1500.0  // value (in  ms) when the controller is centered
#define RANGE          500.0  // value (in  ms) of the range from the RC controller

/* Pins used for outputs to the motor controller */
#define LEFT_MOTOR_OUT    9  // confirmed pin  9 is pwm output
#define RIGHT_MOTOR_OUT  10  // confirmed pin 10 is pwm output
// #define AUX_MOTOR_OUT    11  // confirmed pin 11 is pwm output

#define DBG_DELAY       500  // minimum ms between debug writes

/* Motors */
Servo left_motor;
Servo right_motor;

/* Variables used for loop timing */
unsigned long t_last;
unsigned long t_next;
unsigned long t_delta;
unsigned long t_dbg = DBG_DELAY;

/* variables for RC inputs */
unsigned long right_stick = 0;
unsigned long left_stick = 0;
unsigned long aux = 0;

/* variables for conversion of inputs to outputs */
float right;
float left;

/* variables for motor output */
unsigned long left_power = CENTER;
unsigned long right_power = CENTER;


void setup() {
  // Configure input pins
  pinMode(LEFT_STICK_IN, INPUT);
  pinMode(RIGHT_STICK_IN, INPUT);
  pinMode(AUX_IN, INPUT);

  // Attach motors to their output pins
  left_motor.attach(LEFT_MOTOR_OUT);
  right_motor.attach(RIGHT_MOTOR_OUT);
  
  // initialize the motors to no power
  left_motor.writeMicroseconds(CENTER); // set to 1500, which is no power
  right_motor.writeMicroseconds(CENTER); // set to 1500, which is no power

  // For debugging output
  Serial.begin(9600);
  t_last = millis();
}


void loop() {
  /* Read the input pins. This takes about 15 ms/pin.
     Note: Reading pins that aren't connected causes long delays.
           Setting the timeout (thrid parameter) helps to limit the delay,
           but can lead to missed reads if set too low.
  */
  right_stick = pulseIn(RIGHT_STICK_IN, HIGH );
  left_stick = pulseIn(LEFT_STICK_IN, HIGH );
  //aux = pulseIn(AUX_IN, HIGH, 50000);

  /* Normalize the inputs to the range of -1.0 to 1.0 */
  if (right_stick == 0) {
    right = 0.0;  // the pulseIn command failed
  } else {
    right = float(right_stick - CENTER)/float(RANGE);
  }
  
  if (left_stick == 0) {
    left = 0.0;  // the pulseIn command failed
  } else {
    left = float(left_stick - CENTER)/float(RANGE);
  }
    
  /* Convert to motor signals */
  left_power  = (unsigned long)((left)*RANGE + CENTER);
  right_power = (unsigned long)((right)*RANGE + CENTER);
  
  /* make sure that the output values are in the proper range */
  left_power = constrain(left_power, CENTER - RANGE, CENTER + RANGE);
  right_power = constrain(right_power, CENTER - RANGE, CENTER + RANGE);
  
  /* Send power to motors */
  left_motor.writeMicroseconds(left_power);
  right_motor.writeMicroseconds(right_power);
  
  /* Optionally write diagnostic information */
  debug_output();

}


void debug_output() {
  // Write diagnostic information.
  // show_inputs-indicates if this should show the input or output variables.
  //             this helps avoiod overloading the serial buffer
  t_next = millis();
  t_delta = t_next - t_last;

  // Only send data to the Serial port if there is enough space in the buffer
  // to avoid blocking the code waiting for the Serial buffer. The code also 
  // waits a minimum interval (DBG_DELAY) between writes to keep the output readable
  if (Serial.availableForWrite()>60 && millis() > t_dbg) {
    t_dbg = millis() + DBG_DELAY;

    Serial.print("Tloop:");
    Serial.print(t_delta);
    Serial.println();

    Serial.print("In: ");
    Serial.print(right_stick);
    Serial.print(",");
    Serial.print(left_stick);
    Serial.print(",");
    Serial.print(aux);
    Serial.println();

    Serial.print("Nrm:");
    Serial.print(right);
    Serial.print(",");
    Serial.print(left);
    Serial.println();

    Serial.print("Out:");
    Serial.print(left_power);
    Serial.print(",");
    Serial.print(right_power);
    Serial.println();

    Serial.print("Tdbg:");
    Serial.print(millis() - t_next);
    Serial.println();
    Serial.println();
  }

  t_last = millis();
}
