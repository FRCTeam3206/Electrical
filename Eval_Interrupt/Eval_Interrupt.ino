const byte buttonPin = 2;
const byte ledPin = 13;
const int tBlank = 500; // milliseconds to blank button input
unsigned long t;        // millis rolls-over in 50 days.  *** caution ***
unsigned long s = 0;    // variable to compare time since button change

volatile boolean ledState = false;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), isrBlink, RISING);
}

void loop() {
    // The elegance is that there's nothing here. No delay()!
    // In itself, this program isn't "useful" but it is an
    // important building block for something a little bigger.
}

void isrBlink() {
    t = millis();       // get current time in ms since start
    if ((t - s) > tBlank) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);  // write led state
      s = t;
    }
}
