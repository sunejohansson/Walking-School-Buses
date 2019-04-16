//Master
#include <SoftwareSerial.h>
#include "FastLED.h"
#define NUM_LEDS 7
#define DATA_PIN 3

//RFID
#include <MFRC522.h>
#include <SPI.h> // Used for communication via SPI with the Module

#define SDAPIN 10 // RFID Module SDA Pin is connected to the UNO 10 Pin
#define RESETPIN 9 // RFID Module RST Pin is connected to the UNO 8 Pin

SoftwareSerial BTSerial(5, 6);
CRGB leds[NUM_LEDS];

int state = 0;

byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number
byte YellowSerialNumber[5] = {0xFA, 0x4C, 0x0B, 0xE4}; // The Tag Serial number we are looking for
byte PinkSerialNumber[5] = {0x36, 0x85, 0x86, 0xAB}; // The Tag Serial number we are looking for

MFRC522 nfc(SDAPIN, RESETPIN); // Init of the library using the UNO pins declared above

void setup() 
{
  SPI.begin();
  Serial.begin(9600);
  
  BTSerial.begin(38400);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
    
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // Start to find an RFID Module
  Serial.println("Looking for RFID Reader");
  nfc.begin();
  byte version = nfc.getFirmwareVersion(); // Variable to store Firmware version of the Module

  // If can't find an RFID Module
  if (! version) {
    Serial.print("Didn't find RC522 board.");
    while (1); //Wait until a RFID Module is found
  }

  // If found, print the information about the RFID Module
  Serial.print("Found chip RC522 ");
  Serial.print("Firmware version: 0x");
  Serial.println(version, HEX);
  Serial.println();
}

void loop()
{
  String GoodTag = "False"; // Variable used to confirm good Tag Detected

  // Check to see if a Tag was detected
  // If yes, then the variable FoundTag will contain "MI_OK"
  FoundTag = nfc.requestTag(MF1_REQIDL, TagData);

  if (FoundTag == MI_OK) {
    delay(200);
    
    // Get anti-collision value to properly read information from the Tag
    ReadTag = nfc.antiCollision(TagData);
    memcpy(TagSerialNumber, TagData, 4); // Write the Tag information in the TagSerialNumber variable

    Serial.println("Tag detected.");
    Serial.print("Serial Number: ");
    
    for (int i = 0; i < 4; i++) { // Loop to print serial number to serial monitor
      Serial.print(TagSerialNumber[i], HEX);
      Serial.print(", ");
    }
    Serial.println("");
    Serial.println();

    // Check if detected Tag has the right Serial number we are looking for
    for (int i = 0; i < 4; i++) {
      if (YellowSerialNumber[i] != TagSerialNumber[i]) {
        break; // if not equal, then break out of the "for" loop
      }
      if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
        GoodTag = "YELLOW";
      }
    }
    for (int i = 0; i < 4; i++) {
      if (PinkSerialNumber[i] != TagSerialNumber[i]) {
        break; // if not equal, then break out of the "for" loop
      }
      if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
        GoodTag = "PINK";
      }
    }
    
    if (GoodTag == "YELLOW") {
      Serial.println("Yellow Card Registered");
      Serial.println();
      BTSerial.write('1');
      
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
    } else if (GoodTag == "PINK") {
      Serial.println("Pink Card Registered");
      Serial.println();
      BTSerial.write('2');
      
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
    else {
      Serial.println("Tag not accepted! (");
      Serial.println();
      delay(500);
    }
  }
}
