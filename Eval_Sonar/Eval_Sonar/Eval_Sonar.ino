/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:

  - LED connected from digital pin 9 to ground

  by Raj Gandhi

Based on
  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial

Uses ultrasonic transducer XL-MaxSonar MB1200.

*/

#include <Adafruit_NeoPixel.h>

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const byte PIXEL_PIN = 6;      // Digital IO pin connected to the NeoPixels.
const byte PIXEL_COUNT = 4;    // Number of RGB NeoPixel LEDs

// Parameter 1 = number of pixels in "strip"
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Color wheel key: Red = 9, Orange = 29, Yellow = 44, Green = 82, Blue = 172, Purple = 210, Pink = 210
byte color = 215; 
// 210 looks blue, is there somehting wrong with red? 
// 225, 254, 9 = red is fine, 29 looks red, not orange
// 44 looks like green, 82 is also green
// 172 is blue, 210 is blue

  
const int blinks = 10;       // number of blinks per trigger
const int threshold = 42;   // distance in "cm" to trigger
int sensorValue = 0;       
int outputValue = 0;       

void setup() {
  // initialize serial communications at 9600 bps:  this could be commented-out in final
  Serial.begin(9600);

  strip.begin();
  strip.show();                 // Initialize all pixels to 'off'
  strip.setBrightness(254);      // Range of 0 to 255.  32 is nice.  More than 150 is bright.             
  strip.setPixelColor(2, 2, 0, 2); 
  strip.show();
// *** there's some sort of odd interaction with setting brightness and using Wheel color. *** //

}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 600);
  
  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.print(outputValue);
  Serial.print("cm");/**/
  
  if (outputValue < threshold)  // sonar distance threshold
  {
    for (int j = 1; j < blinks; j++){
      for (int i = 0; i < PIXEL_COUNT; i++){
        strip.setPixelColor(i, 255, 0, 255); 
      }
//      strip.setBrightness(254);      // Range of 0 to 255.  32 is nice.  More than 150 is bright.             
      strip.show();                      // Sends updated pixel color to hardware.
      delay(100);
//      strip.setBrightness(1);
      for (int i = 0; i < PIXEL_COUNT; i++){
        strip.setPixelColor(i, 2, 0, 2); 
      }
      strip.show();                      // Sends updated pixel color to hardware.
      delay(100);      
    }
    delay(2000);  //minimize re-triggers
  }
  else{
    delay (100);
  }
 Serial.print("\t color value = ");
 Serial.println(color);
  
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
