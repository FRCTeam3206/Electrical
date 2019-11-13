#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define THROTTLE_PIN   4 // throttle channel from RC receiver
#define STEERING_PIN   5 // steering channel from RC receiver
#define LASER_PIN     11 // laser pointers pin (they share one pin)
#define LED_PIN       13 // user LED pin

#define MAX_SPEED             400 // max motor speed; actual max is 400
#define K_STEERING            0.65 // speed multiplier to decrease steering sensitivity.  0.65 gives about a 4" turn radius at max throttle
#define PULSE_WIDTH_DEADBAND  25  // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define PULSE_WIDTH_RANGE     400 // pulse width difference from 1500 us to be treated as full scale input (for example, a value of 350 means
                                  //  any pulse width <= 1150 us or >= 1850 us is considered full scale)

/* Can't use serial output because boosted battery can smoke comms. */
 
ZumoBuzzer buzzer;              // Enables the Zumo buzzer.
Pushbutton button(ZUMO_BUTTON); // Enables the Zumo button.

int volume = 15;                // 15 is maximum, 10 isn't very loud
long K_SPEED = 8.0;             // Useful range 2 < 32.  16 is sluggish.  no tipping with 8.0  const accel multiplier to decrease max accel in a different way. 
struct voltage{                 // This defines a new data type similar to an array with two int elements.
  int ones;                     // This declares a type "voltage" and calls one of them "Vbat".  
  int tenths;                   // Within the code it should be called Vbat.ones and Vbat.tenths
} Vbat;  
unsigned long timeElapsed = 0;
unsigned long timeRSL = 0;
bool RSL = false;
bool flagLowVbat = false;
long accelLimitedThrottle;      // Range will be +/- 400.  No real value to give it an initial value?

/* Postpone: Try to move button detect to interrupt.  ==> Not possible without HW change!
 *  Pushbutton is hardwired to DIO 12.  On an Uno, only DIO pins 2 and 3 can attach to interrupt.
 *  Pushbutton.h goes to signficant effort to capture a button push event in a polled paradigm.
 *  It also appears to do a good job of debouncing.  However, it is unlikely that an interrupt 
 *  wouldn't be superior.  There's lots of code running and it would be ~easy to miss a button
 *  push event.
*/

int laserIntensity = 165;                    // this is pretty bright but doesn't waste power
int left_speed, right_speed;                 // Variables for motor outputs

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
  checkVbat();                          // Check battery voltage if button is pressed or 30s elapsed.
  timeRSL = indicatorRSL(timeRSL);      // RSL is health state indicator   
/*  
 *   RC input and motor output
 */
  int throttle = pulseIn(THROTTLE_PIN, HIGH);  // Read throttle input.  Normally 1000 to 2000
  int steering = pulseIn(STEERING_PIN, HIGH);  // Read steering input.  Normally 1000 to 2000
  if (throttle > 0 && steering > 0) // Absense of PWM signal results in 0.
  { // RC signals encode information in pulse width centered on 1500 us (microseconds); subtract 1500 to get a value centered on 0
        steering -= 1500;
        throttle -= 1500;
    // Apply Deadband -- Deadband not needed for controllers with good trim?
    if (K_SPEED == 0.0)
      K_SPEED = 1.0;
    
    accelLimitedThrottle = (accelLimitedThrottle/K_SPEED*(K_SPEED-1.0)) + throttle/K_SPEED;   

//    if (abs(throttle) <= PULSE_WIDTH_DEADBAND)
//      throttle = 0;
//    else
//      accelLimitedThrottle = (accelLimitedThrottle/4.0*3.0) + throttle/4.0;   

    
    if (abs(steering) <= PULSE_WIDTH_DEADBAND)
      steering = 0;
    else
      steering *= K_STEERING;      // Apply simple scaling to keep steering reasonable
      
    // mix throttle and steering inputs to obtain left & right motor speeds
    left_speed = ((long)accelLimitedThrottle * MAX_SPEED / PULSE_WIDTH_RANGE) - ((long)steering * MAX_SPEED / PULSE_WIDTH_RANGE);
    right_speed = ((long)accelLimitedThrottle * MAX_SPEED / PULSE_WIDTH_RANGE) + ((long)steering * MAX_SPEED / PULSE_WIDTH_RANGE);

    // cap speeds to max
    left_speed = min(max(left_speed, -MAX_SPEED), MAX_SPEED);  // max is 400
    right_speed = min(max(right_speed, -MAX_SPEED), MAX_SPEED); // max is 400
  }
  else     // At least one RC signal is not good; turn off LED and stop motors
   {
    left_speed = 0;
    right_speed = 0;
   }

  ZumoMotors::setSpeeds(left_speed, right_speed);  // In setSpeeds source code, max is +/- 400
}

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

void  checkVbat() {
  bool flagManVbat = false;           // local variable
  bool flagAutoVbat = false;          // local variable

  if (button.isPressed()) {
    playWelcome();                    // play welcome tones to acknowledge button press
    flagManVbat = true;
  }
  if ((millis() - timeElapsed) > 30000) { // Poll voltage very 30 seconds
    flagAutoVbat = true;
  }
  if (flagManVbat or flagAutoVbat) {
     Vbat = readVbat();               // Read battery voltage
     if (Vbat.ones < 6) {             // 6V is low; 5.6 would be dangerously low
       if (not(flagLowVbat)){
         analogWrite (LASER_PIN, 0);  // Turn off laser pointers to save power.
         flagLowVbat = true;          // This is global variable prevents unnecessary re-writes to laser
       }
     }
    timeElapsed = millis();
    if ((flagLowVbat and flagAutoVbat) or flagManVbat) playVbat(Vbat);  // Beep out battery voltage.
  }
}

/*
 * RSL : LED on in 13 indicates health.
 * ToDo: Finish full implementation of five RSL states
 * and add another RSL LED
 */

unsigned long indicatorRSL (unsigned long timeRSLLocal){  // This would make more sense if there were 
  if (millis() - timeRSLLocal > 1000) {                   // a separate "RSL" LED.
    RSL = not(RSL); 
    timeRSLLocal = millis();
  }
  if (RSL)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
}
