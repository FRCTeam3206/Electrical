#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define THROTTLE_PIN   4 // throttle channel from RC receiver
#define STEERING_PIN   5 // steering channel from RC receiver
#define LED_PIN       13 // user LED pin

#define MAX_SPEED             800 // max motor speed
#define PULSE_WIDTH_DEADBAND   25 // pulse width difference from 1500 us (microseconds) to ignore (to compensate for control centering offset)
#define PULSE_WIDTH_RANGE     350 // pulse width difference from 1500 us to be treated as full scale input (for example, a value of 350 means
                                  //   any pulse width <= 1150 us or >= 1850 us is considered full scale)

ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);
int volume = 15;                //(15 is maximum, 10 isn't very loud)

// To do: move Vbat tones to a separate function.  Try to move button detect to interrupt.

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
}

void loop()
{

  if (button.isPressed())
  {
    int SensorRead = analogRead(A1);      // there's a 1/2 hardware voltage divider, a max read of 1023 corresponds to 10V
    //  int SensorRead = 900;               // fake this input to get the math to  work.  900 is about 8.789V, 800 is 7.813V, 700 is 6.836V
    int x = SensorRead * 10.0 / 1024;     // works;  x = 8
    int y = SensorRead * 10 % 1024;       // works;  trying to make y equal to tenths of a volt
    y = y + 1024/20;                      // works;  add half a volt to compensate for future trunc  
    y = y * 10;                           // works;  mult x 10 to compensate for future div
    y = y / 1024;                         // works;  final div yields tenths of volt

    buzzer.playNote(NOTE_A(6), 100, volume);
    while (buzzer.isPlaying());
    buzzer.playNote(NOTE_B(6), 100, volume);
    while (buzzer.isPlaying());
    buzzer.playNote(SILENT_NOTE, 500, volume);
    while (buzzer.isPlaying());

    //static void playNote(unsigned char note, unsigned int duration, unsigned char volume);

    for (int i = 1; i<=x; i++){  
        buzzer.playNote(NOTE_A(5), 250, volume);
        while (buzzer.isPlaying());
        buzzer.playNote(SILENT_NOTE, 250, volume);
        while (buzzer.isPlaying());
    }
    if (y < 1) buzzer.playNote(NOTE_A(2), 250, volume);
    
    for (int i = 1; i<=y; i++){  
        buzzer.playNote(NOTE_B(5), 250, volume);
        while (buzzer.isPlaying());
        buzzer.playNote(SILENT_NOTE, 250, volume);
        while (buzzer.isPlaying());
    }

   }

  
  int throttle = pulseIn(THROTTLE_PIN, HIGH);
  int steering = pulseIn(STEERING_PIN, HIGH);

  int left_speed, right_speed;



  if (throttle > 0 || steering > 0)
  {
    // both RC signals are good; turn on LED
    digitalWrite(LED_PIN, HIGH);

    // RC signals encode information in pulse width centered on 1500 us (microseconds); subtract 1500 to get a value centered on 0
    throttle -= 1500;
    steering -= 1500;

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
    digitalWrite(LED_PIN, LOW);

    left_speed = 0;
    right_speed = 0;
  }

  ZumoMotors::setSpeeds(left_speed, right_speed);
}
