#include <ZumoBuzzer.h>
#include <Pushbutton.h>

/*
 * This example uses the ZumoBuzzer library to play a series of notes on
 * the buzzer.  It also uses Pushbutton library to allow the user to
 * stop/reset the melody with the user pushbutton.
 */

ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);
int volume = 15;                //(15 is maximum loud, 10 isn't very loud)
struct voltage{                 // This declares a type "voltage" and calls one of them "Vbat".  
  int ones;                     // Within the code it should be called Vbat.ones and Vbat.tenths
  int tenths;
} Vbat;  

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);
}

void loop()                     // run over and over again
{
  int z;
  
//  int SensorRead = analogRead(A1);      // there's a 1/2 hardware voltage divider, a max read of 1023 corresponds to 10V
  int SensorRead = 700;               // fake this input to get the math to  work.  900 is about 8.789V, 800 is 7.813V, 700 is 6.836V
  int x = SensorRead * 10.0 / 1024;     // works;  x = 8
  int y = SensorRead * 10 % 1024;       // works;  trying to make y equal to tenths of a volt
  y = y + 1024/20;                      // works;  add half a volt to compensate for future trunc  
  y = y * 10;                           // works;  mult x 10 to compensate for future div
  y = y / 1024;                         // works;  final div yields tenths of volt
  
  delay (500);

  Vbat = readVbat(56);
Serial.print(x); Serial.print("."); Serial.println(y);
Serial.print(Vbat.ones); Serial.print("."); Serial.println(Vbat.tenths);


  if (button.isPressed())
  {
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
}

voltage readVbat (int sensorValue){
  voltage V; //declare V for use inside this function only
  V.ones = sensorValue;
  V.tenths = sensorValue / 10;
  return V; 
}


