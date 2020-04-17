/**
 * SCOREBOARD
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * Compatible with MCU:
 *  ARDUINO MEGA AVR 2560
 *  ADAFRUIT M0 SAMD21
 *  ADAFRUIT Grand Central SAMD51
 *  
 * Pixel Driver Shield V1.0
 *  switches
 * 
 * W2812B LEDs
 * 
 * nRF24L01 2.4GHz Radio
 * 
 */

 //DEBUG
bool debug = true;
#define POST false  //Power On Self Test (POST). Runs test patterns in setup()

//BOARD
//#define MEGA2560 //5V

//PROCESSOR
//#define SAMD51   //3.3V

//SHIELD
#define SCOREBOARD_PROTOYPE true  //prototype 5V.  (No RTC, EEPROM. No mode switch)
#define PIXELDRIVER true         //PCB: 9-CH, RTC, EEPROM, Mode switch.  5V or 3.3V

//FONTS
#include "fonts.h"      //custom fonts for 5x7 pixels arrays

//LEDs
#include <Adafruit_NeoPixel.h>

//PINOUTS 
//each one of these is a string of 35 LEDs (except colon 2 pixels)
                       //PIN  //function
#define SEC01          31     //timer digit 0 seconds 1's right most
#define SEC10          33     //timer digit 1 seconds 10's
#define MIN01          35     //timer digit 2 minutes 1's
#define MIN10          37     //timer digit 3 minutes 10's left most
#define COLON          39     //timer colon  //2 pixels only
#define HOM01          45//41   //score home 1's 
#define HOM10          47//43   //score home 10's
#define VIS01          41//45   //score visitors 1's  
#define VIS10          43//47   //score visitors 10's 
//                            //
#define NUMPIXELS      35     //max number of pixels in a digit (5 x 7 = 35 pixels per digit)
#define BRIGHTNESS     192    //a common way to set a max color value from 0 (off) to 255 (full on)

//Array of pixels - the "digit" objects                                                 //DIGIT
Adafruit_NeoPixel digSec01 = Adafruit_NeoPixel(NUMPIXELS, SEC01, NEO_RGB + NEO_KHZ800); //0
Adafruit_NeoPixel digSec10 = Adafruit_NeoPixel(NUMPIXELS, SEC10, NEO_RGB + NEO_KHZ800); //1
Adafruit_NeoPixel digMin01 = Adafruit_NeoPixel(NUMPIXELS, MIN01, NEO_RGB + NEO_KHZ800); //2
Adafruit_NeoPixel digMin10 = Adafruit_NeoPixel(NUMPIXELS, MIN10, NEO_RGB + NEO_KHZ800); //3
Adafruit_NeoPixel digColon = Adafruit_NeoPixel(2        , COLON, NEO_RGB + NEO_KHZ800); //4   //2 pixels only
Adafruit_NeoPixel digHom01 = Adafruit_NeoPixel(NUMPIXELS, HOM01, NEO_RGB + NEO_KHZ800); //5
Adafruit_NeoPixel digHom10 = Adafruit_NeoPixel(NUMPIXELS, HOM10, NEO_RGB + NEO_KHZ800); //6
Adafruit_NeoPixel digVis01 = Adafruit_NeoPixel(NUMPIXELS, VIS01, NEO_RGB + NEO_KHZ800); //7
Adafruit_NeoPixel digVis10 = Adafruit_NeoPixel(NUMPIXELS, VIS10, NEO_RGB + NEO_KHZ800); //8

//NEO LOCAL (ON-BOARD) NEOPIXELS (3 qty) 
#define NEO_LOCAL_PIN      49  
Adafruit_NeoPixel neoLocal = Adafruit_NeoPixel(3, NEO_LOCAL_PIN, NEO_RGB + NEO_KHZ800);     //3 pixels on-board
#define NEO_LOCAL_BRT     255

//BUTTONS
#define BUTTON_MODE A12           //MEGA pins
#define BUTTON_TIMER   A13        // "    "
#define BUTTON_VISITOR A14        //
#define BUTTON_HOME    A15        // "    "
int8_t buttonM = HIGH;            //mode      The state of each button (released = HIGH, pressed = LOW) 
int8_t buttonH = HIGH;            //home       "     "      "     "
int8_t buttonV = HIGH;            //visitor    "     "      "     "
int8_t buttonC = HIGH;            //clock      "     "      "     "
int32_t buttonM_hold = 0;         //mode      The number of milliseconds the button is being held "pressed (LOW)"
int32_t buttonH_hold = 0;         //home       "     "      "     "
int32_t buttonV_hold = 0;         //visitor    "     "      "     "
int32_t buttonC_hold = 0;         //clock      "     "      "     "
uint8_t buttonC_hold_sec = 0;     //button clock time second counter


//MODE
//determines what the board should do (e.g. scoreboard, clock...)
//mode is persistant until changed (run many)
#define NOTHING     0   //do nothing. all off, quiet mode, power savings
#define STANDBY     1   //mostly off (except for colon)
#define TEST        2
#define TIMER       3   //timer only  
#define SCOREBOARD  4   //scores and timer
#define CLOCK       5   //time only
#define DATE        6   //date only
#define CLOCK_SET   7   //clock is uninitialized 
#define DATE_SET    8   //time and date
#define RTC         9   //show RTC 
#define RTC_SET     10  //RTC is uninitialized, show interface to set RTC
#define BUZZER      11  //activates buzzer
#define TEMP        12  //temperature
#define TUNER       13  //radio tuner AM/FM
#define TICKER      14  //stock ticker    
#define RANGE       15  //wireless range testing     
#define MODE_MIN   SCOREBOARD
#define MODE_MAX   DATE
int mode = CLOCK;       //select one of the above modes
int mode_last = -1;     //remembers the last mode. Used to detect changes or first time through. Default - unitialized 
int flag_test = 0;      //flag for test mode

//COMMAND
//commands are cleared at the end of each loop (run once)
char cmdByte = NULL;              //this holds the active command byte.  
char cmdStr[32] = "";             //        "         "           str


//SCORE
int score_home = 0;               //0 to 99
int score_visitors = 0;           //"    "
//int flag_home = 0;              //1=refresh
//int flag_visitors = 0;          //1=refresh
int flag_scoreboard = 0;          //1=refresh


//CLOCK - TIMER, DATE, TIME
#include "clock.h"
MyDateTime dt;                    
int flag_clock = 0;                //1=refresh
int flag_timer = 0;                //1=refresh

//RTC
int flag_rtc = 0;                 //1=detected, 0=not found
char rtcZulu[] = "20190701T120304Z";
char * rtcZulu_ptr = rtcZulu;

//BUZZER relay
#define BUZZER A7                 //MEGA analog pin A7 OUTPUT
#define BUZZER_SEC 3000           //on time. The max number of milliseconds the buzzer is allowed on
bool flag_buzzer = false;         //flag to turn buzzer ON/OFF
unsigned long timerBuzzer;        //number of milliseconds since the buzzer was turned on


//RANGE
int range_c = 0;                  //a generic counter used for testing the RF24L01 wireless remote's range


//PERFORMANCE MONITORS
unsigned long loop_timer = millis();
  
/**
 * setup
 */
void setup() {

  //SERIAL
  Serial.begin(115200);   //CONSOLE USB - GRAND CENTRAL - THIS DOES NOT GO TO A PIN, it goes to the CONSOLE ONLY
  Serial1.begin(115200);  //DEBUG - GRAND CENTRAL - THIS GOES TO TX0/RX0 
  delay(1500);
  Serial.println("\nSCOREBOARD with PIXEL DRIVER board V2.0 \nsetup()...");
  Serial1.println("\nDEBUG...");  

  //LED
  pinMode (LED_BUILTIN, OUTPUT);            //enable built-in LED (typically D13)

  //BUTTONS
  pinMode (BUTTON_MODE, INPUT_PULLUP);      //button - score home
  pinMode (BUTTON_HOME, INPUT_PULLUP);      //button - score home
  pinMode (BUTTON_VISITOR, INPUT_PULLUP);   //button - score visitors
  pinMode (BUTTON_TIMER, INPUT_PULLUP);     //button - clock

  //BUZZER 
  pinMode (BUZZER, OUTPUT);                 //this drives a SPDT relay circuit
  digitalWrite(BUZZER, HIGH);               //init to off.  Active low. 0=ON, 1=OFF
         
  //nRF24L01
  setup_RF24L01();                          //setup nRF24

  //RTC
  if(PIXELDRIVER){
    flag_rtc = setup_rtc();                 //detect and setup, I2C device
  }

  //EEPROM
  if(PIXELDRIVER){
   setup_eeprom();                          //detect and setup, I2C device
  }

  //DIGITS
  digSec01.begin();  //seconds 1's
  digSec10.begin();  //seconds 10's
  digMin01.begin();  //minutes 1's
  digMin10.begin();  //minutes 10's
  digColon.begin();  //colon (2 pixels)
  digHom01.begin();  //score home 1's
  digHom10.begin();  //score home 10's
  digVis01.begin();  //score visitors 1's
  digVis10.begin();  //score visitors 10's

  //NEOPIXELS
  neoLocal.begin();  //PIXEL DRIVER 3-pixels on-board
  for(int j=0; j<64; j++){
    for(int i=0; i<3; i++){
      neoLocal.setPixelColor(i, random(0,255), random(0,255), random(0,255)); 
      neoLocal.show(); 
    }
    delay(50);
  }
  //OFF
  for(int i=0; i<3; i++){
      neoLocal.setPixelColor(i,0,0,0);    
  }
  neoLocal.show(); 
 
  //POWER-ON SELF TEST - run test pattern(s) - also helpful to indicate unexpected reboots
  if(POST) {
    random_beauty();
    //primary_colors();
    //countdown_digit_test();
  }


  //SCOREBOARD
  if(mode == SCOREBOARD){
    scoreboard_init();        //scoring
    dt.timer_init();             //timing
  }

  //DATE TIME 
  if(flag_rtc){  //flag_rtc
    //
    getRTCZulu(rtcZulu_ptr);
    dt.initZulu(rtcZulu);             //override - set date and time
  }
  dt.timer_sec = millis();     //begin clock/cal timebase


  //TIMER
  if(0){
    dt.timer_init();             //this feature is part of the scoreboard
  }

}//end setup


/** 
 * loop
 * controls -> command -> mode -> timer or clock -> display/buzzer -> repeat
 */
void loop() {
  
  //loop monitor
  loop_timer = millis();


  //CONTROLS
  if(1){   
    loop_controls();            //always read pending commands from the buttons, switches each time through loop
  }


  //nRF24
  if(1) {
    cmdByte = loop_RF24L01();   //always read pending commands from the nRF24L01
  }


  //COMMANDS - byte
  switch (cmdByte) {            //process any pending commands

    //mode - increment
    case 'M' : {mode = -1;}     //resets mode to 0 when 'm' command runs. No break here, continue...
    case 'm' : {
        mode++;
        if(mode > MODE_MAX){
          mode = MODE_MIN;
        }
        break;
      }

    //mode - set to scoreboard or clock
    case 'S' : {mode = SCOREBOARD;/*scoreboard_init();*/ break;}   //set mode to SB and init
    case 's' : {mode = CLOCK;     /*dt.clock_init();*/ break;}          //force mode to clock (not SB)
    case 'D' : {mode = DATE_SET; break;}
    case 'd' : {mode = DATE; break;}

    //home
    case 'H' : {score_home = -1;}     //resets score to 0 when 'h' command runs. No break here, continue...
    case 'h' : {
      if(mode == SCOREBOARD){
          score_home++;
          if(score_home > 99){score_home = 0;}
          draw_score_home();
          break;
        }
        if(mode == DATE || DATE_SET){
          dt.set_month();       //increment 
          break;
        }
        if(mode == CLOCK || mode == CLOCK_SET){
          dt.set_hours();       //increment 
          break;
        }     
      }
      
    //visitor 
    case 'V' : {score_visitors = -1;}  //resets score to 0 when 'v' command runs. No break here, continue...
    case 'v' : {
        if(mode == SCOREBOARD){
          score_visitors++;
          if(score_visitors > 99){score_visitors = 0;}
          draw_score_visitors();
          break;
        }
        if(mode == DATE || DATE_SET){
          dt.set_day();       //increment 
          break;
        }
        if(mode == CLOCK || mode == CLOCK_SET){
          dt.set_minutes();     //increment
          break;
        }
      }

    //time set to known state  
    case 'O' :{ timerOnOff = 1; break;}  //timer on
    case 'o' :{ timerOnOff = 0; break;}  //timer off
    
    //timer and clock reset
    case 'C' : {
        if(mode == DATE || mode == DATE_SET || mode == CLOCK || mode == CLOCK_SET){
          Serial.println("RESETTING DATE/CLOCK: ");
          //timerOnOff = 0;                     //set to off    
          dt.initZulu("20190701T120304Z");      //YYYYMMddThhmmss, July 1, 2019 12:03.04 PM
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
        dt.calc_timer2minsec();
        dt.draw_timer();       //refresh timer display after reset or when timer timer is OFF
        Serial.print("TIMER RESET TO: "); Serial.println(timerMillisClock);
        break;
      }
      
     //timer and clock toggle start/stop
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
           
    //test or demo mode
    case 't' : {
        Serial.print("TEST: ");
        //int timerOnOffTemp = timerOnOff; timerOnOff = 0; //save timer setting, the disable timer
        random_beauty();
        //primary_colors();
        //countdown_digit_test();
        //clear_all();
        //dt.draw_timer();
        //timerOnOff = timerOnOffTemp; //restore timer setting
        break;
      }
      
    //RANGE range test mode (for development only)
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

    //CLOCK AND CALENDAR
    case 'Z' : {
        //dt.loop_date();      //date
        break;
      }
    case 'z' : {
        dt.loop_time();      //time
        break;
      }


    //X CURSOR
    case 'X' : {
        score_home++;
        draw_score_home();
        break;
      }
    case 'x' : {
        score_home--;
        draw_score_home();
        break;
      }

    //Y CURSOR
    case 'Y' : {
        score_visitors++;
        draw_score_visitors();
        break;
      }
    case 'y' : {
        score_visitors--;
        draw_score_visitors();
        break;
      }
    
  }//end switch


  //COMMANDS - string
  

  //MODE 
  if(mode != mode_last){        //detect mode changes, set flags to force refresh
    Serial.print("MODE: ");      
    switch(mode){
      case NOTHING    : {Serial.println("NOTHING");break;}
      case STANDBY    : {Serial.println("STANDBY");break;}
      case SCOREBOARD : {Serial.println("SCOREBOARD");flag_scoreboard = 1;break;}
      case CLOCK      : {Serial.println("CLOCK");     flag_clock = 1;break;}
      case CLOCK_SET  : {Serial.println("CLOCK_SET"); flag_clock = 1;break;}
      case DATE       : {Serial.println("DATE");      flag_clock = 1;break;}
      case DATE_SET   : {Serial.println("DATE_SET");  flag_clock = 1;break;}
      case TIMER      : {Serial.println("TIMER");break;}
      //case CALENDAR   : {Serial.println("CALENDAR");break;}
      //case RTC        : {Serial.println("RTC_SET");break;}   
      case TEST       : {Serial.println("TEST");flag_test = 1;break;}
      default         : {Serial.println("INVALID");}
    }
  }


  /**
   * RENDERING
   */

  //STANDBY
  if(mode == STANDBY){
    clear_all();
    mode = NOTHING;
  }

  //SCOREBOARD
  if(mode == SCOREBOARD){
    /*
    if(flag_timer){dt.draw_timer();}              
    if(flag_home){draw_score_home();}
    if(flag_visitors){draw_score_visitors();}
    */
    //draw only when flagged
    if(flag_scoreboard){
      dt.draw_timer();
      draw_score_home();
      draw_score_visitors();
      flag_scoreboard = 0;
    }
  }

   //RTC SET
  if(mode == RTC_SET){
    //TODO - user interace to set RTC
  }

  //RTC
  if(mode == RTC){

    //loop_rtc();              //display RTC values
    Serial.print("RTCZ: ");
    Serial.println(rtcZulu);

    flag_rtc = 0;               //clear
  }

  //CLOCK SET
  if(mode == CLOCK_SET){
    dt.dtInit();                //a user interface for setting the clock
    //dt.loop_time();             //refresh clock
  }

  //CLOCK 
  if(mode == CLOCK){
    dt.loop_time();             //refresh clock
  }

  //DATE SET
  if(mode == DATE_SET){
    dt.dtInit();               //a user interface for setting the date
    //dt.loop_date();            //refresh date
  }

  //DATE 
  if(mode == DATE){
    dt.loop_date();             //refresh date
  }

  //TIMER
  //TIMER TICK - establishes the number of times the timer will update per second 
  if ((mode == SCOREBOARD || mode == TIMER) && timerOnOff && (millis() - timerElapsed) > TIMER_TICK) {

    //LIMIT - check for limit beyond MAX
    if (timerMillisClock > 5400000 ) {
      timerMillisClock = 5400000 ;
    }

    //TICK - decrement the timer's time remaining by one timer tick
    timerMillisClock -= TIMER_TICK;   //subtracts a timer tick (milliseconds) from the time remaining
    timerElapsed = millis();          //reset elapsed time since for the next timer tick
    
    //RENDER - draw the timer upon each tick
    //flag_timer = 1;
    //or
    dt.draw_timer();
    
    //EXPIRED - if timer is on and remaining time is zero, then time just expired. Trigger buzzer
    if (timerMillisClock <= 0 ) {
      Serial.println("TIMER EXPIRED");
      timerOnOff = 0;                 //STOP timer
      flag_buzzer = true;             //buzzer to ON
      timerBuzzer = millis();
    }
  }


  //BUZZER
  if(flag_buzzer && ((millis() - timerBuzzer) < BUZZER_SEC)){
    //ON
    Serial.print("BUZZER ON "); Serial.print(millis() - timerBuzzer); Serial.println(" msec");
    digitalWrite(BUZZER, HIGH);  
  } else {
    //OFF
    digitalWrite(BUZZER, LOW);
    flag_buzzer = false; 
    timerBuzzer = millis();
    //Serial.println("BUZZER OFF "); 
  }


  //CLEAR
  cmdByte = NULL;             //clear the active command each time through the main loop (so commands only run once)
  strcpy(cmdStr,NULL);
  mode_last = mode;           //detect mode changes


  //LOOP
  if(0){
    //loop performance monitor
    Serial.print("loop_timer: ");Serial.print(millis() - loop_timer);  Serial.print(" msec"); 
  }

}//end main loop
