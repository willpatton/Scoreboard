/**
 * SCOREBOARD CONTROLLER
 * 
 * BOARD: ADAFRUIT FEATHER 32u4 
 * RADIO: nRF24L01
 */


/**
 * Arduino Wireless Communication Tutorial
 *     Example 1 - Transmitter Code
 *                
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 * 
 * Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//RF24 radio(7, 8); // CE, CSN  //MEGA
RF24 radio(5, 6); // CE, CSN    //RADIO to FEATHER  pins
const byte address[6] = "00001";

 
//COMMANDS
char command = NULL;

//BUTTONS
#define BUTTON_CLOCK   A5         // "    "
#define BUTTON_HOME    A4         // "    "
#define BUTTON_VISITOR A3         //FEATHER pin
int8_t buttonH = HIGH;            //home      The state of each button (released = HIGH, pressed = LOW) 
int8_t buttonV = HIGH;            //visitor    "     "      "     "
int8_t buttonC = HIGH;            //clock      "     "      "     "
int32_t buttonH_hold = 0;         //home      The number of milliseconds the button is being held "pressed (LOW)"
int32_t buttonV_hold = 0;         //visitor    "     "      "     "
int32_t buttonC_hold = 0;         //clock      "     "      "     "


/**
 * setup
 */
void setup() {
  
  Serial.begin(115200);
  delay(1600);                    //wait for Serial to awake
  Serial.println("Begin Scoreboard Controller");

  //PINS
  pinMode (13, OUTPUT); //LED - onboard LED
  pinMode (BUTTON_HOME, INPUT_PULLUP);
  pinMode (BUTTON_CLOCK, INPUT_PULLUP);
  pinMode (BUTTON_VISITOR, INPUT_PULLUP);

  pinMode (5, OUTPUT);  //radio  CE (chip enable) pin connected to FEATHER D5
  pinMode (6, OUTPUT);  //radio CSN (chip select) pin connected to FEATHER D6

  //RADIO - setup
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}//end setup


/**
 * loop
 */
void loop() {

  /**
     BUTTONS - poll buttons on circuit board
     Used for development (or without controller)
  */
  //HOME - score button
  if (digitalRead(BUTTON_HOME) == LOW && buttonH == HIGH) { //detect press
    buttonH = LOW;
    command = 'h';      //score
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_HOME) == HIGH && buttonH == LOW) { //detect release
    buttonH = HIGH;
    buttonH_hold = 0;
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_HOME) == LOW && buttonH == LOW) { //detect hold
    buttonH_hold++;
    delay(5); //slow down hold sampling
    if (buttonH_hold > 1000) {
      buttonH_hold = 0;
 //     score_home = -1; //this will reset the score to zero when the command runs below
      command = 'H';
    }
  }
  //VISITORS - score button
  if (digitalRead(BUTTON_VISITOR) == LOW && buttonV == HIGH) { //detect press
    buttonV = LOW;
    command = 'v';    //score
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_VISITOR) == HIGH && buttonV == LOW) { //detect release
    buttonV = HIGH;
    buttonV_hold = 0;
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_VISITOR) == LOW && buttonV == LOW) { //detect hold
    buttonV_hold++;
    delay(5); //slow down hold sampling
    if (buttonV_hold > 1000) {
      buttonV_hold = 0;
//      score_visitors = -1; //this will reset the score to zero when the command runs below
      command = 'V';
    }
  }
  //CLOCK - button
  if (digitalRead(BUTTON_CLOCK) == LOW && buttonC == HIGH) { //detect press
    buttonC = LOW;
    command = 'c';    //toggle clock on/off
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_CLOCK) == HIGH && buttonC == LOW) { //detect release
    buttonC = HIGH;
    buttonC_hold = 0;
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_CLOCK) == LOW && buttonC == LOW) { //detect hold
    buttonC_hold++;
    delay(3);         //slow down hold sampling
    if (buttonC_hold > 1000) {
      buttonC_hold = 0;
      command = 'C';  // 'C'lear clock
    }
  }
  //MULTIPLE BUTTONS
  if (digitalRead(BUTTON_VISITOR) == LOW && digitalRead(BUTTON_CLOCK) == LOW && digitalRead(BUTTON_HOME) == LOW) {
    command = 't';    // 't' run test pattern
  }

  /**
     COMMANDS - send commands
  */
  switch (command) {
    //score home
    case 'h' : {
        Serial.println("HOME SCORE + 1: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<h";  //send "h" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //score home RESET
    case 'H' : {
        Serial.println("HOME RESET: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<H";  //send "H" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //score visitors
    case 'v' : {
        Serial.println("VISITOR SCORE + 1: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<v";  //send "v" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //score visitors RESET
    case 'V' : {
        Serial.println("VISITOR RESET: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<V";  //send "V" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
     //clock start/stop
    case 'c' : {  
        Serial.println("CLOCK START/STOP: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<c";  //send "c" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //clock RESET
    case 'C' : {
        Serial.println("CLOCK RESET: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<C";  //send "C" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //test or demo mode
    case 't' : {
        Serial.println("TEST/DEMO: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<t";  //send "t" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
    //RANGE testing
    case 'r' : {
        Serial.println("RANGE TEST: ");
        digitalWrite(13, HIGH);
        const char text[] = "$SB<r";  //send "r" command toward the SB $SB<
        radio.write(&text, sizeof(text));
        digitalWrite(13, LOW);
        delay(500);
        break;
      }
  }
  //CLEAR - clear the active command
  command = NULL;
  
}
