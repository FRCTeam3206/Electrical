#include <ZumoBuzzer.h>
#include <Pushbutton.h>

/*
 * This example uses the ZumoBuzzer library to play a series of notes on
 * the buzzer.  It also uses Pushbutton library to allow the user to
 * stop/reset the melody with the user pushbutton.
 */

ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);

void setup()                    // run once, when the sketch starts
{

}

void loop()                     // run over and over again
{
  if (button.isPressed())
  {
    //static void playNote(unsigned char note, unsigned int duration, unsigned char volume);
    for (int i = 0; i<=9; i++){  
      buzzer.playNote(NOTE_A(4), 250, 10);
      while (buzzer.isPlaying());
      buzzer.playNote(SILENT_NOTE, 100, 10);
      while (buzzer.isPlaying());
//      buzzer.playNote(NOTE_A(6), 250, 10);
//      while (buzzer.isPlaying());
    }
   }
  
}

