/*********************************************************************
This is an example for our nRF8001 Bluetooth Low Energy Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1697

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Kevin Townsend/KTOWN  for Adafruit Industries.
MIT license, check LICENSE for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

// This version uses the internal data queing so you can treat it like Serial (kinda)!

#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);

// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
/**************************************************************************/
/*!
    Configure the Arduino and start advertising with the radio
*/

int ledarray[] = {0,0,0, 0, 0, 0, 0, 0 ,0};
char temp[] = "";
int count = 0;
int countLed = 0; 
char st[] = ",";

/**************************************************************************/
void setup(void)
{ 
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Print echo demo"));

  // BTLEserial.setDeviceName("NEWNAME"); /* 7 characters max! */

  BTLEserial.begin();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

/**************************************************************************/
/*!
    Constantly checks for new events on the nRF8001
*/
/**************************************************************************/
aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void loop()
{
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Advertising started"));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us!
    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
      count = 0;
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial.available()) {


      char c = BTLEserial.read();
      temp[count] = c;
      count++;
      Serial.print(c);
      if (c == *st){
        switch (countLed){
        case 0:
          ledarray[0] = atoi(temp);
          Serial.print("ledarray[0] = ");
          Serial.print(ledarray[0]);
          countLed ++;
          count = 0;
          break;
        case 1:
          ledarray[1] = atoi(temp);
          Serial.print("ledarray[1] = ");
          Serial.print(ledarray[1]);
          countLed ++;
          count = 0;  
          break;
        case 2:
          ledarray[2] = atoi(temp);
          Serial.print("ledarray[2] = ");
          Serial.print(ledarray[2]);
          countLed ++;
          count = 0;
          break;
         case 3:
          ledarray[3] = atoi(temp);
          Serial.print("ledarray[3] = ");
          Serial.print(ledarray[3]);
          countLed ++;
          count = 0;
          break;
        case 4:
          ledarray[4] = atoi(temp);
          Serial.print("ledarray[4] = ");
          Serial.print(ledarray[4]);
          countLed ++;
          count = 0;  
          break;
        case 5:
          ledarray[5] = atoi(temp);
          Serial.print("ledarray[5] = ");
          Serial.print(ledarray[5]);
          countLed ++;
          count = 0;
          break;
        case 6:
          ledarray[6] = atoi(temp);
          Serial.print("ledarray[6] = ");
          Serial.print(ledarray[6]);
          countLed ++;
          count = 0;
          break;
        case 7:
          ledarray[7] = atoi(temp);
          Serial.print("ledarray[7] = ");
          Serial.print(ledarray[7]);
          countLed ++;
          count = 0;  
          break;
        case 8:
          ledarray[8] = atoi(temp);
          Serial.print("ledarray[8] = ");
          Serial.print(ledarray[8]);
          countLed = 0;
          count = 0;
          break;
        }
      }
//      delay(500);
      
      for(int i = 0; i < 6; i++){
        if(ledarray[i+3] == 1){ 
        strip.setPixelColor(i, ledarray[0], ledarray[1] , ledarray[2]);
        strip.show();
        }else{
          strip.setPixelColor(i, 0, 0, 0);
        strip.show();
        } 
      }
    }

    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(100); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);
    }
  }
}
