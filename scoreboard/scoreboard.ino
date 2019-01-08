/**
 * SCOREBOARD
 * 
 * AUTHOR: Will Patton. http://willpatton.com
 * 
 * BOARD: ARDUINO MEGA 2560, nRF24L01 radio
 * 
 */

//DEBUG
bool debug = true;

#include <Adafruit_NeoPixel.h>
#include "fonts.h"

//DIGITS                    //MEGA pinout assignments
#define SEC01          31   //clock digit 0 seconds 1's right most
#define SEC10          33   //clock digit 1 seconds 10's
#define MIN01          35   //clock digit 2 minutes 1's
#define MIN10          37   //clock digit 3 minutes 10's left most
#define COLON          39   //clock colon
#define HOM01          41   //score home 1's 
#define HOM10          43   //score home 10's
#define VIS01          45   //score visitors 1's  
#define VIS10          47   //score visitors 10's 
                            //
#define NUMPIXELS      35   //max number of pixels in a digit (5 x 7 = 35 pixels per digit)
#define BRIGHTNESS     192   //a common way to set a max color value from 0 (off) to 255 (full on)

//Instantiate "digit" objects                                                           //DIGIT
Adafruit_NeoPixel digSec01 = Adafruit_NeoPixel(NUMPIXELS, SEC01, NEO_RGB + NEO_KHZ800); //0
Adafruit_NeoPixel digSec10 = Adafruit_NeoPixel(NUMPIXELS, SEC10, NEO_RGB + NEO_KHZ800); //1
Adafruit_NeoPixel digMin01 = Adafruit_NeoPixel(NUMPIXELS, MIN01, NEO_RGB + NEO_KHZ800); //2
Adafruit_NeoPixel digMin10 = Adafruit_NeoPixel(NUMPIXELS, MIN10, NEO_RGB + NEO_KHZ800); //3
Adafruit_NeoPixel digColon = Adafruit_NeoPixel(NUMPIXELS, COLON, NEO_RGB + NEO_KHZ800); //4
Adafruit_NeoPixel digHom01 = Adafruit_NeoPixel(NUMPIXELS, HOM01, NEO_RGB + NEO_KHZ800); //5
Adafruit_NeoPixel digHom10 = Adafruit_NeoPixel(NUMPIXELS, HOM10, NEO_RGB + NEO_KHZ800); //6
Adafruit_NeoPixel digVis01 = Adafruit_NeoPixel(NUMPIXELS, VIS01, NEO_RGB + NEO_KHZ800); //7
Adafruit_NeoPixel digVis10 = Adafruit_NeoPixel(NUMPIXELS, VIS10, NEO_RGB + NEO_KHZ800); //8

//COMMAND and CONTROL
char command = NULL;              //this holds the active command.  (e.g. 'c' to start/stop timer. 'h' to increment the home team score

//BUTTONS
#define BUTTON_VISITOR A15        //MEGA pin
#define BUTTON_CLOCK   A14        // "    "
#define BUTTON_HOME    A13        // "    "
int8_t buttonH = HIGH;            //home      The state of each button (released = HIGH, pressed = LOW) 
int8_t buttonV = HIGH;            //visitor    "     "      "     "
int8_t buttonC = HIGH;            //clock      "     "      "     "
int32_t buttonH_hold = 0;         //home      The number of milliseconds the button is being held "pressed (LOW)"
int32_t buttonV_hold = 0;         //visitor    "     "      "     "
int32_t buttonC_hold = 0;         //clock      "     "      "     "

//SCORING
int score_home = 0;               //0 to 99
int score_visitors = 0;           //"    "

//CLOCK
int min10 = '0';                  //minute 10's
int min01 = '0';                  //minute 1's
int sec10 = '0';                  //second 10's
int sec01 = '0';                  //second 1's
//int tenths = '0';
//int hundreths = '0';

//TIMERS
#define TIMER_TICK 1000           //The number of milliseconds in a timer tick. Used to establish a time base (refresh rate). 
                                  //(1000 = TIMER_TICK updates  1Hz once per second)
                                  //( 100 = TIMER_TICK updates 10Hz 10 times per second)
//#define CLOCK_RESET 3600000;      //3600000 = 60 min * 60 sec * 1000 milliseconds 
//#define CLOCK_RESET 1200000;      //1200000 = 20 min * 60 sec * 1000 milliseconds 
//#define CLOCK_RESET 900000;       // 900000 = 15 min * 60 sec * 1000 milliseconds 
//#define CLOCK_RESET 600000;       // 600000 = 10 min * 60 sec * 1000 milliseconds 
//#define CLOCK_RESET 300000;       // 300000 =  5 min * 60 sec * 1000 milliseconds
#define CLOCK_RESET 60000;        //  60000 =  1 min * 60 sec * 1000 milliseconds
//#define CLOCK_RESET 30000;        //   2000 =  0 min * 20 sec * 1000 milliseconds
//#define CLOCK_RESET 20000;        //   2000 =  0 min * 20 sec * 1000 milliseconds
int timerOnOff = 0;               //indicates if clock is ON or OFF. 1 = clock ON (running), 0 = clock OFF (stopped)
unsigned long timerMillisClock;   //millisecond timer used to keep the clock's time remaining
unsigned long timerElapsed = 0;   //millisecond timer to keep track of elapsed time (used by clock)

//OTHER
int range_c = 0;                  //a generic counter used for testing the range

/**
   setup
*/
void setup() {

  //SERIAL
  Serial.begin(115200);
  delay(1500);
  Serial.println("SCOREBOARD V.01 - begin setup()");

  //LED
  pinMode (13, OUTPUT);           //enable onboard LED at digital pin 13

  //BUTTONS
  pinMode (BUTTON_VISITOR, INPUT_PULLUP);   //button - score visitors
  pinMode (BUTTON_CLOCK, INPUT_PULLUP);     //button - clock
  pinMode (BUTTON_HOME, INPUT_PULLUP);      //button - score home

  //DIGITS - instantiate digit objects
  digSec01.begin();  //clock seconds 1's
  digSec10.begin();  //clock seconds 10's
  digMin01.begin();  //clock minutes 1's
  digMin10.begin();  //clock minutes 10's
  digColon.begin();  //clock colon
  digHom01.begin();  //score home 1's
  digHom10.begin();  //score home 10's
  digVis01.begin();  //score visitors 1's
  digVis10.begin();  //score visitors 10's

  //POWER-ON test pattern for all pixels - also indicates reboots
  if(0) {
    random_beauty();
    primary_colors();
    countdown_digit_test();
  }

  //RADIO - RF24L01 
  setup_RF24L01();                //set radio to receiver mode

  //SCORES
  draw_score_home();
  draw_score_visitors();

  //TIMERS - timers
  timerMillisClock = CLOCK_RESET; //reset the clocks main timer. Holds the time remaining in milliseconds (e.g. 900000 milliseconds = 15 minutes)
  draw_clock();                   //render clock face to setup() values
  timerElapsed = millis();        //initialize clock's timer
 
}//end setup


/**
  loop
      control -> command -> display -> repeat
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
    delay(5); //slow down hold sampling
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
     RADIO - read radio receiver for pending commands
  */
  if (int cmd = loop_RF24L01()) {
    command = cmd;
  }


  /**
     COMMANDS - process any pending commands
  */
  switch (command) {
    //score home
    case 'H' : {score_home = -1;}     //resets score to 0 when 'h' command runs. No break here, continue...
    case 'h' : {
        score_home++;
        draw_score_home();
        break;
      }
    //score visitors
    case 'V' : {score_visitors = -1;}  //resets score to 0 when 'v' command runs. No break here, continue...
    case 'v' : {
        score_visitors++;
        draw_score_visitors();
        break;
      }
     //clock start/stop
    case 'c' : {
        //if time remaining, then it's okay to start/stop the clock here
        if (timerMillisClock) {
          timerOnOff = timerOnOff ^ 1;    //bitwise toggle
        } else {
          //if there is no time remaining, the clock's timer can only be off/zero/stopped
          timerOnOff = 0;
        } 
        Serial.print("CLOCK: ");
        if (timerOnOff) {
          Serial.println("ON");
        } else {
          Serial.println("OFF");
        } break;
      }
    //clock reset
    case 'C' : {
        timerMillisClock = CLOCK_RESET; /*reset clock*/  timerOnOff = 0; /*clear clock to OFF*/ timer2minsec();
        draw_clock();         //refresh clock display after reset or when timer clock is OFF
        Serial.println("CLOCK RESET");
        break;
      }
    //test or demo mode
    case 't' : {
        Serial.print("TEST: ");
        int timerOnOffTemp = timerOnOff; timerOnOff = 0; //save timer setting, the disable timer
        random_beauty();
        //primary_colors();
        //countdown_digit_test();
        clear_all();
        draw_clock();
        timerOnOff = timerOnOffTemp; //restore timer setting
        break;
      }
    //range test mode (for development only)
    case 'r' : {
        //RANGE - continously increments a digit if the transmitter is within range
        if (range_c > 9) {
          range_c = 0;
        }
        if (range_c < 0) {
          range_c = 9;
        }
        //clear_all();
        draw(range_c + 0x30, 0);   //render the clock digit in ASCII
        digSec01.show();
        range_c--;
        break;
      }
  }
  //CLEAR - clear the active command each time through the main loop (so new commands only run once)
  command = NULL;


  /**
   *  CLOCK
   */
  //TIMER TICK - establishes the number of times the clock will update per second 
  if (timerOnOff && (millis() - timerElapsed) > TIMER_TICK) {

    //if time remaining is zero or less, then do nothing here, return.
    if (timerMillisClock <= 0 ) {
      return;
    }

    //decrement the clock's time remaining by one timer tick
    timerMillisClock -= TIMER_TICK;   //subtracts a timer tick (milliseconds) from the time remaining
    timerElapsed = millis();    //reset elapsed time since for the next timer tick

    //stop clock if time remaining is zero or below
    if (timerMillisClock <= 0) {
      timerOnOff = 0;  // STOP clock
      //timerMillisClock = CLOCK_RESET;
    }

    //render the clock upon each clock tick
    draw_clock();
  }


}//end main loop

