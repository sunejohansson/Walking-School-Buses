//Slave
#include <SoftwareSerial.h>
#include "FastLED.h"
#define NUM_LEDS 7
#define DATA_PIN 3

SoftwareSerial BTSerial(10, 11);
CRGB leds[NUM_LEDS];

int state = 0;

void setup() 
{
  BTSerial.begin(38400);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}
void loop() 
{
 if(BTSerial.available() > 0)
 { 
    state = BTSerial.read(); // Reads the data from the serial port
    
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Orange;
    FastLED.show();
    delay(70);
    }
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(70);
    }
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Orange;
    FastLED.show();
    }
    delay(1700);
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(70);
    }
 }
 
 if (state == '1') 
 {
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(50);
    }
  state = 0;
 }
 else if (state == '2') 
 {  
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Red;
    FastLED.show();
    }
    delay(250);
    for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
    FastLED.show();
    }
    delay(250);
    state = 0;
  }
}
