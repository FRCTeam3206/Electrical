#include <Servo.h>

/*
  Minimal program to control a robot with a RC Pistol Grip
*/

/* Pins used for inputs from the RC receiver */
#define STEERING_IN      5  // steering channel from RC receiver
#define THROTTLE_IN      4  // throttle channel from RC receiver
#define AUX_IN           6  // aux channel from RC receiver

#define CENTER        1500.0  // value (in  ms) when the controller is centered
#define RANGE          500.0  // value (in  ms) of the range from the RC controller

/* Pins used for outputs to the motor controller */
#define LEFT_MOTOR_OUT    9  // confirmed pin  9 is pwm output
#define RIGHT_MOTOR_OUT  10  // confirmed pin 10 is pwm output
// #define AUX_MOTOR_OUT    11  // confirmed pin 11 is pwm output

/* Motors */
Servo left_motor;
Servo right_motor;

/* Variables used for loop timing */
unsigned long t_last;
unsigned long t_next;
unsigned long t_delta;

/* variables for RC inputs */
unsigned long throttle = 0;
unsigned long steering = 0;
unsigned long aux = 0;

/* variables for conversion of inputs to outputs */
float forward;
float rotate;

/* variables for motor output */
unsigned long left_power = CENTER;
unsigned long right_power = CENTER;


void setup() {
  Serial.begin(9600);

  // Configure input pins
  pinMode(STEERING_IN, INPUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(AUX_IN, INPUT);

  // Attach motors to their output pins
  left_motor.attach(LEFT_MOTOR_OUT);
  right_motor.attach(RIGHT_MOTOR_OUT);
  
  // initialize the motors to no power
  left_motor.writeMicroseconds(CENTER); // set to 1500, which is no power
  right_motor.writeMicroseconds(CENTER); // set to 1500, which is no power

  t_last = millis();
}


void loop() {
  /* Read the input pins. This takes about 15 ms/pin.
     Note: Reading pins that aren't connected causes long delays.
           Setting the timeout (thrid parameter) helps to limit the delay,
           but can lead to missed reads if set too low.
  */
  throttle = pulseIn(THROTTLE_IN, HIGH, 50000);
  steering = pulseIn(STEERING_IN, HIGH, 50000);
  //aux = pulseIn(AUX_IN, HIGH, 50000);

  /* Normalize the inputs to the range of -1.0 to 1.0 */
  forward = -float(throttle - CENTER)/float(RANGE);
  rotate = float(steering - CENTER)/float(RANGE);

  forward = constrain(forward, -1.0, 1.0);
  rotate = constrain(rotate, -1.0, 1.0);
  
  /* Convert to motor signals */
  left_power  = (unsigned long)((forward + rotate)*RANGE + CENTER);
  right_power = (unsigned long)((forward - rotate)*RANGE + CENTER);
  
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
  // to avoid blocking the code waiting for the Serial buffer
  if (Serial.availableForWrite()>60) {
    Serial.print("Tloop:");
    Serial.print(t_delta);
    Serial.println();

    Serial.print("In: ");
    Serial.print(throttle);
    Serial.print(",");
    Serial.print(steering);
    Serial.print(",");
    Serial.print(aux);
    Serial.println();

    Serial.print("Nrm:");
    Serial.print(forward);
    Serial.print(",");
    Serial.print(rotate);
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
