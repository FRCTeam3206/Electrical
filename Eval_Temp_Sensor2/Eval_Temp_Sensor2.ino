/*To do list:
 * add the button with interrrupt and change LED color for a set duration
 * add the state machine 
 *    functions first and then the case structure?
 * add relay and LED to verify functionality
 * add filtering to temperature input IIR 1/8 or could be an array of 100 readings
 * 
 * The relay data sheet needs 72mA.  Arduino per pin max is 40mA (20mA recommended).
 * Need a driver, MOSFET or motor shield.
 * 
 */
 
#include <Adafruit_NeoPixel.h>

//TMP36 Pin (temperature sensor)
const byte sensorPin  = 0;     //the analog pin the TMP36's Vout (sense) pin is connected to
                               //the resolution is 10 mV / degree centigrade with a
                               //500 mV offset to allow for negative temperatures
 
const byte BUTTON_PIN = 2;     // Digital IO pin connected to the button.  This will be
                               // driven with a pull-up resistor so the switch should
                               // pull the pin to ground momentarily.  On a high -> low
                               // transition the button press logic will execute.

const byte PIXEL_PIN = 6;      // Digital IO pin connected to the NeoPixels.
const byte PIXEL_COUNT = 1;    // Number of RGB NeoPixel LEDs

const byte HEATER_PIN = 8;      // Digital IO pin for the relay (which powers heating lamp).

// State Machine States
const byte tempOk = 0;         // Temperature is within limits.
const byte tooCold = 1;        // Temperature is too cold.
const byte tooHot = 2;         // Temperature is too hot.  Future use.
const byte prog = 3;           // Program set point.
const byte error = 4;          // Error state (developer error).

// Parameter 1 = number of pixels in "strip"
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Color wheel key: Red = 9, Orange = 29, Yellow = 44, Green = 82, Blue = 172, Purple = 210, Pink = 210
// Color mapping to temperature formula
const int t1 = 18;             // deg C, low temperature  *** change this after development ***
const int t2 = 25;             // deg C, high temperature *** change this after development ***
const byte c1 = 225;           // cold color (purple)
const byte c2 = 0;             // hot color (red)
float m;                       // y = mx + b where y is color and x is temperature
float b;                       // m & b are globals. Calculated in setup() and used in loop().

bool oldState = HIGH;          // Button state
int showType = 0;              // relevant to NeoPixel 
float setPointHeater = 23;     // Initial heater set point
float hysteresis = 2;          // Overshoot a little to prevent heater oscillation
int fsm_state = 0;             // State machine current state, init to tempOk
bool heater = LOW;             // Heater state.  Init to off.


void setup() {
  Serial.begin(9600);          //Start the serial connection with the computer
                               //to view the result open the serial monitor 
                       
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(HEATER_PIN, OUTPUT);
  strip.begin();
  strip.show();                 // Initialize all pixels to 'off'
  strip.setBrightness(32);      // Range of 0 to 255.  32 is nice.  More than 150 is bright.             

// NeoPixel Color formula:
  m = (c2 -c1) / (t2 - t1);
  b = c1 - m * t1;
 
}   // End of setup()

void loop() {
 // Get voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);    //Do an analog read of sensorPin and assign value to reading
  float temperature = out_text(reading);  //Call function out_text with value of "reading"; returns temperature
  out_led(temperature);                   //Call subroutine out_led with value of "temperature"

  switch (fsm_state)
  {
    case tempOk:                          // previous state was tempOk
      if (temperature < setPointHeater)
      {
        fsm_state = tooCold;              // too cold, next state is tooCold
        digitalWrite(HEATER_PIN, HIGH);   // Reconsider if HIGH = heater On;
      } 
      else if (temperature > setPointHeater)
      {
        fsm_state = tempOk;               // temperature is still ok
      }
      break;
    case tooCold:                         // previous state was tooCold
      if (temperature < (setPointHeater + hysteresis))   //overshoot setPoint by a little bit to prevent oscillation
      {
        fsm_state = tooCold;              // too cold, next state is tooCold
      } 
      else if (temperature > setPointHeater)
      {
        fsm_state = tempOk;               // temperature is still ok
        digitalWrite(HEATER_PIN, LOW);  
      }
      break;
    case tooHot:
      break;
    case prog:
      break;
    case error:
      break;
    default:
      break;
  }
  Serial.println(fsm_state);
  delay(1000);      


}

float out_text (int reading) {
 // converting that reading to voltage, for 3.3v arduino use 3.3
   float voltage = reading * 5.0;
   voltage /= 1024.0;  
 
   Serial.print(voltage); Serial.print(" V  ");            //print voltage
   float temperatureC = (voltage - 0.5) * 100 ;            //converting from 10 mv per degree wit 500 mV offset
                                                           //to degrees ((voltage - 500mV) times 100)
   Serial.print(temperatureC); Serial.print(" deg C  ");   //print temperature C
  
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0; //now convert to Fahrenheit
   Serial.print(temperatureF); Serial.print(" deg F");   //print temperature F
  return float (temperatureC);  
}

void out_led (float temperature) {
   int color = m * temperature + b;   // color = -5 * temp + 300
   
   if (color < 0) {color = 0;}        // don't allow out of bounds colors 
   if (color > 225) {color = 225;}    // don't allow out of bounds colors
   
   Serial.print("  color "); Serial.println(color);             // Print numeric color
   strip.setPixelColor(0, Wheel (color)); 
   strip.show();                      // Sends updated pixel color to hardware.
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
