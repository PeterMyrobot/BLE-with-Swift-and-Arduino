#include <Adafruit_NeoPixel.h>

#define PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  for(int i = 0; i < 6; i++){

      strip.setPixelColor(i, 255, 255 , 255);
   strip.show();
  }

   

}
