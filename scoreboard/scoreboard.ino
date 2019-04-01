/**
 * SCOREBOARD
 * 
 * AUTHOR: Will Patton. http://willpatton.com
 * 
 * BOARD: 
 * ARDUINO MEGA 2560
 * nRF24L01 2.4GHz Radio
 * 
 */

 //DEBUG
bool debug = true;
#define POST false  //Power On Self Test (POST). Run test patterns

//MODE
#define SCOREBOARD  1   //SCOREBOARD & TIMER
#define CLOCK       2   //CLOCK
#define DATE        3   //CALENDAR
#define CLOCK_SET   4   //CLOCK is uninitialized 
//#define RADIO     5   //RADIO STATION
//#define TICKER    6   //STOCK TICKER
int mode = CLOCK;  //choose one of the above modes

//LEDs
#include <Adafruit_NeoPixel.h>
#include "fonts.h"

//TIMER DIGITS                    //MEGA pinout assignments
#define SEC01          31   //timer digit 0 seconds 1's right most
#define SEC10          33   //timer digit 1 seconds 10's
#define MIN01          35   //timer digit 2 minutes 1's
#define MIN10          37   //timer digit 3 minutes 10's left most
#define COLON          39   //timer colon
#define HOM01          45//41   //score home 1's 
#define HOM10          47//43   //score home 10's
#define VIS01          41//45   //score visitors 1's  
#define VIS10          43//47   //score visitors 10's 
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
#define BUTTON_VISITOR A13        //MEGA pin
#define BUTTON_TIMER   A14        // "    "
#define BUTTON_HOME    A15        // "    "
int8_t buttonH = HIGH;            //home      The state of each button (released = HIGH, pressed = LOW) 
int8_t buttonV = HIGH;            //visitor    "     "      "     "
int8_t buttonC = HIGH;            //clock      "     "      "     "
int32_t buttonH_hold = 0;         //home      The number of milliseconds the button is being held "pressed (LOW)"
int32_t buttonV_hold = 0;         //visitor    "     "      "     "
int32_t buttonC_hold = 0;         //clock      "     "      "     "
uint8_t buttonC_hold_sec = 0;     //button clock time second counter

//SCORES
int score_home = 0;               //0 to 99
int score_visitors = 0;           //"    "

//CLOCK & CALENDAR DIGITS
int year_ = 0;                   //year  yyyy
int month_ = 0;                  //month mm
int day_ = 0;                    //day   dd
int hours = 0;                  //time
int minutes = 0;
int seconds = 0;

//timer - digits
int hour10 = 0;                  //hour 10's
int hour01 = 0;                  //hour 1's
int min10  = 0;                  //minute 10's
int min01  = 0;                  //minute 1's
int sec10  = 0;                  //second 10's
int sec01  = 0;                  //second 1's
//int tenths = '0';
//int hundreths = '0';

//BUZZER
#define BUZZER A7                 //MEGA analog pin 7 OUTPUT
#define BUZZER_SEC 3000           //max number of milliseconds the buzzer is allowed on
bool buzzerSound = false;         //flag to turn buzzer ON/OFF
unsigned long timerBuzzer;        //number of milliseconds since the buzzer was turned on

//MILLISECONDS
#define TIMER_TICK 1000           //The number of milliseconds in a timer tick. Used to establish a time base (refresh rate). 
                                  //(1000 = TIMER_TICK updates  1Hz once per second)
                                  //( 100 = TIMER_TICK updates 10Hz 10 times per second)
#define TIMER_MIN_90  5400000;      //5400000 = 90 min * 60 sec * 1000 milliseconds 
#define TIMER_MIN_5  300000;        //300000  =  5 min * 60 sec * 1000 milliseconds 
#define ADD_TIME_TO_TIMER TIMER_MIN_5;    

//DEFAULT TIMER VALUES upon RESET (uncomment the value most often used. e.g. 20 min for a hockey period)
//#define TIMER_RESET 5400000;      //5400000 = 90 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 3600000;      //3600000 = 60 min * 60 sec * 1000 milliseconds 
  #define TIMER_RESET 1200000;      //1200000 = 20 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 900000;       // 900000 = 15 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 600000;       // 600000 = 10 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 300000;       // 300000 =  5 min * 60 sec * 1000 milliseconds
//#define TIMER_RESET 60000;        //  60000 =  1 min * 60 sec * 1000 milliseconds
//#define TIMER_RESET 30000;        //  30000 =  0 min * 30 sec * 1000 milliseconds
//#define TIMER_RESET 20000;        //  20000 =  0 min * 20 sec * 1000 milliseconds

//TIMERS
int timerOnOff = 0;               //indicates if timer is ON or OFF. 1 = timer ON (running), 0 = timer OFF (stopped)
unsigned long timerMillisClock;   //millisecond timer used to keep the SB timer's time remaining
unsigned long timerElapsed = 0;   //millisecond timer to keep track of elapsed time (used by timer)

//ZULU TIME
String time_zulu;
unsigned long timerTimeZulu;


//OTHER
int range_c = 0;                  //a generic counter used for testing the wireless remote's range


/**
 * setup
 */
void setup() {

  //SERIAL
  Serial.begin(115200);
  delay(1500);
  Serial.println("SCOREBOARD V0.2 - begin setup()");

  //LED
  pinMode (LED_BUILTIN, OUTPUT);           //enable built-in LED

  //BUTTONS
  pinMode (BUTTON_VISITOR, INPUT_PULLUP);   //button - score visitors
  pinMode (BUTTON_HOME, INPUT_PULLUP);      //button - score home
  pinMode (BUTTON_TIMER, INPUT_PULLUP);     //button - clock

  //BUZZER 
  pinMode (BUZZER, OUTPUT);  
  digitalWrite(BUZZER, LOW);   
         
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

  //RADIO - RF24L01 
  setup_RF24L01();                //set radio to receiver mode

  //POWER-ON SELF TEST - run test pattern(s) - also helpful to indicate unexpexted reboots
  if(POST) {
    random_beauty();
    primary_colors();
    countdown_digit_test();
  }

  //MODE
  /*
  switch(mode){
    case CLOCK : {Serial.println("MODE: CLOCK");break;}
    default : {Serial.println("MODE: SCOREBOARD & TIMER");break;}
  }
  */
  if(mode == SCOREBOARD){
    Serial.println("MODE: SCOREBOARD & TIMER");
    //INIT SCORES
    draw_score_home();
    draw_score_visitors();
  
    //INIT TIMER
    timerMillisClock = TIMER_RESET; //reset the clocks main timer. Holds the time remaining in milliseconds (e.g. 900000 milliseconds = 15 minutes)
    draw_timer();                 //render timer's digits to setup() values
    timerElapsed = millis();        //init clock's timer
    timerBuzzer = millis();         //init clock's buzzer
      }

  //CLOCK - time and date
  if(mode == CLOCK /*&& mode != CLOCK_SET*/){
    Serial.println("MODE: CLOCK");
    //Go clock "zulu" time
    //command = 'z';
    timerTimeZulu = millis(); //init clock/cal timebase
    setup_time("00000101T120000Z");      //Jan 1, 0000 12:00.00 pm
    //setup_time("20190309T183500Z");   //Mar 9, 2019  6:35.00 pm  //HARDCODED override for debug
  }
  
}//end setup


/** 
 * loop
 * 
 * control -> command -> clock (display/buzzer) -> repeat
 */
void loop() {

  /**
     BUTTONS - poll buttons on circuit board
     Used for development (or without controller)
  */
  //HOME - score button, hours button
  if (digitalRead(BUTTON_HOME) == LOW && buttonH == HIGH) { //detect press
    buttonH = LOW;
    //if(mode == SCOREBOARD){
      command = 'h';      //home score
    //}
    //if(mode == CLOCK){
    //  command == 'n';   //n for hours
    //}  
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
    //if(mode == SCOREBOARD){
      command = 'v';    //vistor's score
    //}
    //if(mode == CLOCK){
    //  command == 'm';  //n for minutes
    //} 
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
  //TIMER - button
  if (digitalRead(BUTTON_TIMER) == LOW && buttonC == HIGH) { //detect press
    buttonC = LOW;
    command = 'c';    //toggle timer on/off
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_TIMER) == HIGH && buttonC == LOW) { //detect release
    buttonC = HIGH;
    buttonC_hold = 0;
    delay(10);        //debounce
  }
if (digitalRead(BUTTON_TIMER) == LOW && buttonC == LOW) { //detect hold
    buttonC_hold++;
    delay(2);         //slow down hold sampling
    if (buttonC_hold > 1000) {
      buttonC_hold = 0;
      command = 'C';  // 'C'lear timer
    }
  }
  //MULTIPLE BUTTONS
  if (digitalRead(BUTTON_VISITOR) == LOW && digitalRead(BUTTON_TIMER) == LOW && digitalRead(BUTTON_HOME) == LOW) {
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
        if(mode == CLOCK || mode == CLOCK_SET){
          hours++;
          break;
        }
        score_home++;
        if(score_home > 99){score_home = 0;}
        draw_score_home();
        break;
      }
    //score visitors
    case 'V' : {score_visitors = -1;}  //resets score to 0 when 'v' command runs. No break here, continue...
    case 'v' : {
        if(mode == CLOCK || mode == CLOCK_SET){
          minutes++;
          break;
        }
        score_visitors++;
        if(score_visitors > 99){score_visitors = 0;}
        draw_score_visitors();
        break;
      }
     //timer start/stop
    case 'c' : {
        if(mode == CLOCK || mode == CLOCK_SET){
          Serial.println("CLOCK: ");
          mode = CLOCK;
          break;
        }    
        //if time remaining, then it's okay to start/stop the timer here
        if (timerMillisClock) {
          timerOnOff = timerOnOff ^ 1;    //bitwise toggle
        }
        Serial.print("TIMER: ");
        if (timerOnOff) {
          Serial.println("ON");
        } else {
          Serial.println("OFF");
        } 
        //reset button timer hold if "c" command detected
        buttonC_hold_sec = 0; 
        break;
      }
    //timer reset
    case 'C' : {
        if(mode == CLOCK || mode == CLOCK_SET){
          Serial.println("RESETTING CLOCK: ");
          //mode = CLOCK;
          timerTimeZulu = millis(); //init clock/cal timebase
          setup_time("00000101T120000Z");      //Jan 1, 0000 12:00.00 pm
          break;
        }
        
        buttonC_hold_sec++;  //inc this counter each time the 'C' command is detected (about once per second)

        if (buttonC_hold_sec > 1 && buttonC_hold_sec < 3) {
          timerMillisClock = 0; //TIMER_RESET; /*reset timer*/ 
        }
        if ((buttonC_hold_sec > 3 ) /*&& (buttonC_hold_sec & 0x01)*/) {  // more than N sec, then every odd numbered second
          timerMillisClock += ADD_TIME_TO_TIMER;
        }
        
        //limit timer to 90 min MAX
        if (timerMillisClock > 5400000 ) {
          timerMillisClock = 5400000 ;
        }   
        timerOnOff = 0;     //set timer to OFF during reset
        timer2minsec();
        draw_timer();       //refresh timer display after reset or when timer timer is OFF
        Serial.print("TIMER RESET TO: "); Serial.println(timerMillisClock);
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
        draw_timer();
        timerOnOff = timerOnOffTemp; //restore timer setting
        break;
      }
    //range test mode (for development only)
    case 'r' : {
        //RANGE - continuously increments a digit if the transmitter is within range
        if (range_c > 9) {
          range_c = 0;
        }
        if (range_c < 0) {
          range_c = 9;
        }
        //clear_all();
        draw(range_c + 0x30, 0);   //render the digit in ASCII
        digSec01.show();
        range_c--;
        break;
      }
      case 'z' : {
        //CLOCK ZULU time 
        loop_time_zulu();
      }
      case 'Z' : {
        //CLOCK ZULU time and date 
        //loop_time_date_zulu();
      }
  }
  //CLEAR - clear the active command each time through the main loop (so commands only run once)
  command = NULL;

  //PATCH - always force command to 'z' in clock mode
  if(mode == CLOCK || mode == CLOCK_SET){
    command = 'z';
  }


  /**
   *  TIMER
   */
  //TIMER TICK - establishes the number of times the timer will update per second 
  if (timerOnOff && (millis() - timerElapsed) > TIMER_TICK) {

    //LIMIT - check for limit beyond MAX
    if (timerMillisClock > 5400000 ) {
      timerMillisClock = 5400000 ;
    }

    //TICK - decrement the timers's time remaining by one timer tick
    timerMillisClock -= TIMER_TICK;   //subtracts a timer tick (milliseconds) from the time remaining
    timerElapsed = millis();          //reset elapsed time since for the next timer tick
    
    //RENDER - draw the timer upon each tick
    draw_timer();

    //EXPIRED - if timer is on and remaining time is zero, then time just expired.
    if (timerMillisClock <= 0 ) {
      Serial.println("TIMER EXPIRED");
      timerOnOff = 0;  // STOP timer
      buzzerSound = true; /*buzzer to ON*/
      timerBuzzer = millis();
      //return;
    }
  }


  //BUZZER
  if(buzzerSound && ((millis() - timerBuzzer) < BUZZER_SEC)){
    //set to ON
    //Serial.print("BUZZER ON "); Serial.print(millis() - timerBuzzer); Serial.println(" msec");
    digitalWrite(BUZZER, HIGH);  
  } else {
    //set to OFF
    digitalWrite(BUZZER, LOW);
    buzzerSound = false; 
    timerBuzzer = millis();
    //Serial.println("BUZZER OFF "); 
  }


}//end main loop

