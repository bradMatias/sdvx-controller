#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            13

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Encoder knobLeft(3, 4);
Encoder knobRight(1, 2);

// not using a loop becuase this method uses less cpu cycles
void setRingPixels( Adafruit_NeoPixel & neo, byte LEDPos ) {
  neo.clear();
  neo.setPixelColor(LEDPos, 255, 0, 255);
  neo.setPixelColor(LEDPos+4, 255, 0, 255);
  neo.setPixelColor(LEDPos+8, 255, 0, 255);
}

void keepPosInRange( float & pos ) {
  byte intPart = (byte) pos;
  if( pos > 3 ) {
    pos = intPart % 3 + pos - intPart;
  } else if( pos < 0 ) {
    pos = 3 - pos; 
  }
}

byte leftLEDPos = 0;
long knobReading = 0; // 96 = 1 spin
float leftKnobPos = 0; // [0, 3]
float leftBrightness = 0; // [0, 1]
const byte baseBrightness = 40;
char leftDirection = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  //Serial.begin(9600);
}

void loop() {
  knobReading = knobLeft.read();
  if( knobReading != 0 ) {
    leftDirection = knobReading > 0 ? 1 : -1;
  }
  leftBrightness += abs( knobReading ) - leftBrightness / 1000;
  //pixels.setBrightness( baseBrightness * pow( leftBrightness / 40, 2 ) );
  pixels.setBrightness( baseBrightness );
  leftKnobPos += ( min( leftBrightness/10, 1 ) * leftDirection ) * 0.01;
  keepPosInRange( leftKnobPos );
  setRingPixels( pixels, round( leftKnobPos ) );
  pixels.show();
  //Serial.println( leftDirection );
  knobLeft.write(0);
}
