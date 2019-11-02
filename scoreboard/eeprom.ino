/**
 * EEPROM
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * ADAFRUIT
 * Hacked code from Adafruit's FRAM example. Need to fix-up!
 * Adafruit_FRAM_I2C.h
 * 
 * SPARKFUN
 * https://learn.sparkfun.com/tutorials/reading-and-writing-serial-eeproms?_ga=2.121879051.1833927645.1558581452-558505842.1518806316
 * 
 * 
 * CHIP: CAT24C64WI-GT3
 * I2C address: 0x50
 */

#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();
uint16_t framAddr = 0;

void setup_eeprom(void) {
  //Serial.begin(115200);
  
  if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM");
  } else {
    Serial.println("No I2C FRAM found ... check your connections\r\n");
    //Serial.println("FOUND: manufID_wcp");Serial.println(manufID_wcp);
    //while (1);
  }

  //DEBUG - Read, Write, Read
  uint8_t wcp = fram.read8(0x0);
  Serial.print("Inital Data is: "); Serial.println(wcp, HEX);
  fram.write8(0x0, 0x44);
  delay(3);
  wcp = fram.read8(0x0);
  Serial.print("Data is:"); Serial.println(wcp, HEX);
   
  //DEBUG - init block of memory
  for (uint16_t a = 0; a < 512; a++) { //32768
    fram.write8(a, a);
    delay(2);
  }
  
  //DEBUG - dump block of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 512; a++) { //32768
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }
}

void loop_eeprom(void) {

}
