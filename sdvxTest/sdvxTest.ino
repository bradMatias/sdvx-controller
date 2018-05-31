#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel leftRing = Adafruit_NeoPixel(12, 12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightRing = Adafruit_NeoPixel(12, 11, NEO_GRB + NEO_KHZ800);

// pin 1 doesn't work well idk why
// pin 2 and 3 are hw interrupt pins, so good to use 1 for each
Encoder knobRight(2, 5);
Encoder knobLeft(3, 4);

// not using a loop becuase this method uses less cpu cycles
void setRingPixels( Adafruit_NeoPixel & neo, byte LEDPos, bool left ) {
  neo.clear();
  if( left ) {
    neo.setPixelColor(LEDPos, 0, 255, 255);
    neo.setPixelColor(LEDPos+4, 0, 255, 255);
    neo.setPixelColor(LEDPos+8, 0, 255, 255);
  } else {
    neo.setPixelColor(LEDPos, 255, 0, 255);
    neo.setPixelColor(LEDPos+4, 255, 0, 255);
    neo.setPixelColor(LEDPos+8, 255, 0, 255);
  }
}

void keepPosInRange( float & pos ) {
  byte intPart = (byte) pos;
  if( pos > 3 ) {
    pos = intPart % 3 + pos - intPart;
  } else if( pos < 0 ) {
    pos = 3 + pos;
  }
}

const byte baseBrightness = 50;

// left ring stuff
float leftReading = 0; // 24 = 1 spin
float leftKnobPos = 0; // [0, 3]
float leftBrightness = 0; // [0, 1]
float leftMoving = 0; // if > 0 then knob is moving
bool leftStopped = false;

// right ring stuff
float rightReading = 0; // 24 = 1 spin
float rightKnobPos = 0; // [0, 3]
float rightBrightness = 0; // [0, 1]
float rightMoving = 0; // if > 0 then knob is moving
bool rightStopped = false;

void setup() {
  leftRing.begin();
  rightRing.begin();
  //Serial.begin(9600);
}

void loop() {
  leftReading = knobLeft.read() / 2.0; // 1 click on encoder = 2.0 raw read
  
  if( leftReading != 0 ) {
    leftMoving += 0.25;
  } else {
    leftMoving -= 0.001;
  }
  leftMoving = constrain( leftMoving, -1, 0.5);
  if( leftMoving > 0 ) {
    leftBrightness += ( baseBrightness - leftBrightness ) * 0.01;
    leftStopped = false;
  } else {
    if( !leftStopped ) {
      leftStopped = true;
      leftMoving = -1;
    }
    leftBrightness *= 0.995;
  }

  rightReading = knobRight.read() / 2.0; // 1 click on encoder = 2.0 raw read
  if( rightReading != 0 ) {
    rightMoving += 0.25;
  } else {
    rightMoving -= 0.001;
  }
  rightMoving = constrain( rightMoving, -1, 0.5);
  if( rightMoving > 0 ) {
    rightBrightness += ( baseBrightness - rightBrightness ) * 0.01;
    rightStopped = false;
  } else {
    if( !rightStopped ) {
      rightStopped = true;
      rightMoving = -1;
    }
    rightBrightness *= 0.995;
  }
  
  leftRing.setBrightness( leftBrightness );
  leftKnobPos += leftReading;
  keepPosInRange( leftKnobPos );
  setRingPixels( leftRing, round( leftKnobPos ), true );
  leftRing.show();

  rightRing.setBrightness( rightBrightness );
  rightKnobPos += rightReading;
  keepPosInRange( rightKnobPos );
  setRingPixels( rightRing, round( rightKnobPos ), false );
  rightRing.show();

  // reset encoder reading each step
  knobLeft.write(0);
  knobRight.write(0);
}
