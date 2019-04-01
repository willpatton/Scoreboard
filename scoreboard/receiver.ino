/*
 * receiver
 * 
 * AUTHOR: Will Patton. http://willpatton.com
 * 
 * Arduino Wireless Communication Tutorial
 *       Example 1 - Receiver Code
 *                
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * 
 * Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 * 
 * BOARD: ARDUINO MEGA 2560
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(46, 48); // CE (chip enable), CSN (chip select). Assign CE, CSN pins to Arduino MEGA //originally was CE=7, CSN=8
const byte address[6] = "00001";


/**
 * setup - put radio into receiver mode
 */
void setup_RF24L01() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);      //set to minimum power level
  radio.startListening();             
}


/**
 * loop - receiver - reads incoming bytes. 
 * Looks for signature $SB<
 */
int8_t loop_RF24L01() {

   int8_t cmd = NULL;         //holds a character to indicate a bytes was received
  
  if (radio.available()) {
    digitalWrite(LED_BUILTIN, HIGH);   //set on-board LED to ON to visually indicate byte(s) was received
       
    //READ - from radio into text buffer
    char text[32] = "";       //text buffer
    radio.read(&text, sizeof(text)); 
    if(debug){Serial.print("RADIO: command received: "); Serial.println(text); }  
    digitalWrite(LED_BUILTIN, LOW);    //set on-board LED to OFF

    //TODO
    //PARSE - received string
    //compare bytes 0 through 3
    //if(strcmp("$SB<", substr(text,0,3)) === 0){ 
      cmd = text[4];          //the 4th byte is the command
    //}
  }

  //The return value will be NULL or the command byte received.
  return cmd;   
}
