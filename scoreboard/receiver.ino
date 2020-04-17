/*
 * nRF24L01 receiver
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * Adapted from:
 * Arduino Wireless Communication Tutorial
 *       Example 1 - Receiver Code               
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * 
 * Library: 
 * TMRh20/RF24, https://github.com/tmrh20/RF24/
 * 
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 nRF(46, 48); // CE (chip enable), CSN (chip select). Assign CE, CSN pins to Arduino MEGA //originally was CE=7, CSN=8
const byte address[6] = "00001";


/**
 * setup - put radio into receiver mode
 */
void setup_RF24L01() {
  nRF.begin();
  nRF.openReadingPipe(0, address);
  nRF.setPALevel(RF24_PA_MIN);      //set to minimum power level
  nRF.startListening();             
}


/**
 * loop - receiver - reads incoming bytes. 
 * Looks for signature $SB<
 */
int8_t loop_RF24L01() {

   uint8_t cmd = NULL;                  //a character that represents the command
  
  if (nRF.available()) {
    digitalWrite(LED_BUILTIN, HIGH);    //set on-board LED to ON to visually indicate byte(s) was received
       
    //READ
    char buffer[32] = "";                 
    nRF.read(&buffer, sizeof(buffer));    //read buffer
    //if(debug){Serial.print("RCVR buffer: "); Serial.println(buffer); }  

    //PARSE COMMANDS
    parseCommands(buffer);
    
    digitalWrite(LED_BUILTIN, LOW);    //set on-board LED to OFF
  }

  //The return value will be NULL or the command byte received.
  return cmd;   
}
