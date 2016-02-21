#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/*========================================================================================
   PixelStrandTest: This code tests a strand to verify that it works before
     moving on to more detailed testing and setup. It basically lights the first
     then second pixels but rotating through R, G, B, and then lights up the whole
     length one pixel at a time, slowly, then other tests.
  ========================================================================================
*/


#define PIN 6
#define NUM_LEDS 180

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  pinMode(13, OUTPUT);  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  // cycle the first pixel through 3 colors then off
  strip.setPixelColor(0,strip.Color(255,0,0)); strip.show(); delay(500);
  strip.setPixelColor(0,strip.Color(0,255,0)); strip.show(); delay(500);
  strip.setPixelColor(0,strip.Color(0,0,255)); strip.show(); delay(500);
  strip.setPixelColor(0,strip.Color(0,0,0)); strip.show(); delay(500);

  // cycle the second pixel through 3 colors then off
  strip.setPixelColor(1,strip.Color(255,0,0)); strip.show(); delay(500);
  strip.setPixelColor(1,strip.Color(0,255,0)); strip.show(); delay(500);
  strip.setPixelColor(1,strip.Color(0,0,255)); strip.show(); delay(500);
  strip.setPixelColor(1,strip.Color(0,0,0)); strip.show(); delay(500);
  
  // light up each pixel in the strand one at a time, red, green, blue
  colorWipe(strip.Color(255, 0, 0), 100); // Red
  colorWipe(strip.Color(0, 255, 0), 100); // Green
  colorWipe(strip.Color(0, 0, 255), 100); // Blue

  // Send a theater pixel chase in...
  theaterChase(strip.Color(0, 0, 127), NUM_LEDS); // Blue
  delay(1000);
  
  rainbow(20);
  rainbowCycle(20);

  // lights out
  colorWipe(strip.Color(0, 0, 0), 100); 

  delay(1000);
  
} // end loop

void blinkLED(int pin, int nTimes) {
  digitalWrite(pin, LOW);
  for(int i=0; i<nTimes; i++) {
    digitalWrite(13, HIGH); 
    delay(300);
    digitalWrite(13, LOW);
    delay(300); 
  }
    
} // end blinkLED

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    } // end q loop
  } // end j loop
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    } // end q loop
  } // end j loop
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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
