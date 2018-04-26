#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define THROTTLE_PIN   4 // throttle channel from RC receiver
#define STEERING_PIN   5 // steering channel from RC receiver
#define LASER_PIN     11 // laser pointers pin (they share one pin)
#define LED_PIN       13 // user LED pin

#define MAX_SPEED             800 // max motor speed
#define K_SPEED               50  // speed multiplier to decrease max speed in a different way.  Not currently used.
#define K_STEERING            20  // speed multiplier to decrease max speed in a different way.
#define PULSE_WIDTH_DEADBAND  25  // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define PULSE_WIDTH_RANGE     350 // pulse width difference from 1500 us to be treated as full scale input (for example, a value of 350 means
                                  //  any pulse width <= 1150 us or >= 1850 us is considered full scale)

/*
 *  Can't use serial output because boosted battery can smoke comms.
 */
 
ZumoBuzzer buzzer;              // Enables the Zumo buzzer.
Pushbutton button(ZUMO_BUTTON); // Enables the Zumo button.

int volume = 15;                // 15 is maximum, 10 isn't very loud
struct voltage{                 // This defines a new data type similar to an array with two int elements.
  int ones;                     // This declares a type "voltage" and calls one of them "Vbat".  
  int tenths;                   // Within the code it should be called Vbat.ones and Vbat.tenths
} Vbat;  
unsigned long timeElapsed = 0;  // Is this a global variable?
unsigned long timeRSL = 0;
bool RSL = false;

/* Postpone: Try to move button detect to interrupt.  ==> Not possible without HW change!
 *  Pushbutton is hardwired to DIO 12.  On an Uno, only DIO pins 2 and 3 can attach to interrupt.
 *  Pushbutton.h goes to signficant effort to capture a button push event in a polled paradigm.
 *  It also appears to do a good job of debouncing.  However, it is unlikely that an interrupt 
 *  wouldn't be superior.  There's lots of code running and it would be ~easy to miss a button
 *  push event.
*/

int laserIntensity = 165; // this is pretty bright but doesn't waste power

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  analogWrite (LASER_PIN, laserIntensity); 

  timeRSL = millis(); //set initial value for RSL timer
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  // RG: motor flip doesn't work currently
  // motors.flipLeftMotor(true);
  // motors.flipRightMotor(true);
}

void loop()
{   
// To do: combine duplicate Vbat code in auto and manual.

  timeElapsed = pollVbat(timeElapsed);  // Auto poll Vbat. Beeps if low.  

  if (button.isPressed()) {
    playWelcome();
    manVbat();    // Manually query Vbat.
  }

  timeRSL = indicatorRSL(timeRSL);      // RSL is health state indicator   

/*  
 *   RC input and motor output
 */
  int throttle = pulseIn(THROTTLE_PIN, HIGH);  // Read throttle input
  int steering = pulseIn(STEERING_PIN, HIGH);  // Read steering input
  int left_speed, right_speed;                 // Variables for motor outputs

  if (throttle > 0 || steering > 0) // This is an OR so that throttle or steering will work to help debugging.
  {
    
    // RC signals encode information in pulse width centered on 1500 us (microseconds); subtract 1500 to get a value centered on 0
    //    steering -= 1500;  // Orig method
    //    throttle -= 1500;   // Orig method

    if (throttle > 1500)
       throttle = (-(throttle - 1500));// * K_SPEED/100;
    else
       throttle = (1500 - throttle);// * K_SPEED/100;

    if (steering > 1500)
       steering = (-(steering - 1500)) * K_STEERING/100;  // Steering was too sensitive so multiply by constant to reduce sensitivity
    else
       steering = (1500 - steering) * K_STEERING/100;
  
    // apply deadband
    if (abs(throttle) <= PULSE_WIDTH_DEADBAND)
      throttle = 0;
    if (abs(steering) <= PULSE_WIDTH_DEADBAND)
      steering = 0;

    // mix throttle and steering inputs to obtain left & right motor speeds
    left_speed = ((long)throttle * MAX_SPEED / PULSE_WIDTH_RANGE) - ((long)steering * MAX_SPEED / PULSE_WIDTH_RANGE);
    right_speed = ((long)throttle * MAX_SPEED / PULSE_WIDTH_RANGE) + ((long)steering * MAX_SPEED / PULSE_WIDTH_RANGE);

    // cap speeds to max
    left_speed = min(max(left_speed, -MAX_SPEED), MAX_SPEED);
    right_speed = min(max(right_speed, -MAX_SPEED), MAX_SPEED);
  }
  else
  {
    // at least one RC signal is not good; turn off LED and stop motors
//    digitalWrite(LED_PIN, LOW);

    left_speed = 0;
    right_speed = 0;
  }

  ZumoMotors::setSpeeds(left_speed, right_speed);
}


/*
 * To do: Combine SensorRead into this function.
 */
 
voltage readVbat (){
  voltage V;                          // declare V for use inside this function only
  int sensorRead = analogRead(A1);
  V.ones = sensorRead * 10.0 / 1024;  // works;
  int y = sensorRead * 10 % 1024;     // works;  trying to make y equal to tenths of a volt
  y = y + 1024/20;                    // works;  add half a volt to compensate for future trunc  
  y = y * 10;                         // works;  mult x 10 to compensate for future div
  V.tenths = y / 1024;                // works;  final div yields tenths of volt
  return V; 
}

void playWelcome () {
    buzzer.playNote(NOTE_A(6), 100, volume);
    while (buzzer.isPlaying());
    buzzer.playNote(NOTE_B(6), 100, volume);
    while (buzzer.isPlaying());
    buzzer.playNote(SILENT_NOTE, 500, volume);
    while (buzzer.isPlaying());
}

void playVbat (voltage Vbat){
    for (int i = 1; i <= Vbat.ones; i++){  
        buzzer.playNote(NOTE_A(5), 200, volume);
        while (buzzer.isPlaying());
        buzzer.playNote(SILENT_NOTE, 100, volume);
        while (buzzer.isPlaying());
    }
    //if (Vbat.tenths < 1) buzzer.playNote(NOTE_A(2), 250, volume);
     for (int i = 1; i < Vbat.tenths; i++){  
        buzzer.playNote(NOTE_B(5), 200, volume);
        while (buzzer.isPlaying());
        buzzer.playNote(SILENT_NOTE, 100, volume);
        while (buzzer.isPlaying());
    }
}

unsigned long pollVbat (unsigned long timeElapsedLocal){
    if ((millis() - timeElapsedLocal) > 30000) // Poll voltage very 30 seconds
    {
      // playWelcome(); // useful for debugging
 //     int sensorRead = analogRead(A1);
      Vbat = readVbat ();
        if (Vbat.ones < 6) // 6V is low; 5.6 would be dangerously low
        {
          analogWrite (LASER_PIN, 0);    
          playVbat(Vbat);  // play battery voltage in tones to help identify low bat
        }
      timeElapsedLocal = millis();
    }
    return timeElapsedLocal;
}

/*
 * RSL : LED on in 13 indicates health.
 * ToDo: Finish full implementation of five RSL states
 */

unsigned long indicatorRSL (unsigned long timeRSLLocal){
  if (millis() - timeRSLLocal > 1000) {    
    RSL = not(RSL); 
    timeRSLLocal = millis();
  }
  if (RSL)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
}

void manVbat(){
//    playWelcome();
//    int sensorRead = analogRead(A1);          // there's a 1/2 hardware voltage divider, a max read of 1023 corresponds to 10V
//    int SensorRead = 700;                 // uncomment this input to test the math.  900 is about 8.789V, 800 is 7.813V, 700 is 6.836V
//    Vbat = readVbat (sensorRead);
    Vbat = readVbat();
    playVbat(Vbat);
}

