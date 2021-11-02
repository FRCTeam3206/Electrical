// Servo - Version: Latest 
#include <Servo.h>

/*
Minimal program to control a robot with a 
*/

/* Pins used for inputs from the RC receiver */
#define STEERING_IN      5  // steering channel from RC receiver
#define THROTTLE_IN      4  // throttle channel from RC receiver
#define AUX_IN           6  // aux channel from RC receiver

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

int throttle = 0;
int steering = 0;
int aux = 0;


void setup() {
  Serial.begin(9600);
  
  // Configure input pins
  pinMode(STEERING_IN, INPUT);
  pinMode(THROTTLE_IN, INPUT);
  pinMode(AUX_IN, INPUT);
  
  // Attach motors to their outputs
  left_motor.attach(LEFT_MOTOR_OUT);
  left_motor.writeMicroseconds(1500); // set to 1500, which is no power
  right_motor.attach(RIGHT_MOTOR_OUT);
  right_motor.writeMicroseconds(1500); // set to 1500, which is no power
  
  t_last = millis();
}

void loop() {
  // Read the input pins. This takes about 15 ms/pin.
  // Note: Reading pins that aren't connected causes long delays.
  throttle = pulseIn(THROTTLE_IN, HIGH);
  steering = pulseIn(STEERING_IN, HIGH);
  //int aux = pulseIn(AUX_IN, HIGH);
  
  // Convert the inputs into the correct motor signals
  
  // Send power to motors
  left_motor.writeMicroseconds(throttle);
  right_motor.writeMicroseconds(throttle);
  
  // Write diagnostic information
  t_next = millis();
  t_delta = t_next-t_last;

  Serial.print(t_delta);
  Serial.print(",");
  Serial.print(throttle);
  Serial.print(",");
  Serial.print(steering);
  Serial.print(",");
  //Serial.print(aux);
  //Serial.print(",");
  Serial.println(millis()-t_next);
  
  t_last = millis();

}
