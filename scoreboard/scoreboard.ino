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
 * 
 * W2812B LEDs
 * 
 * nRF24L01 2.4GHz Radio (option)
 *
 * EEPROM (option)
 * 
 * Raspberry Pi interface (host interface option)
 *
 */
 
//TIME ZULU
//#define TIME_TO_SET "20200101T120000Z"

 //DEBUG
bool debug = false;
#define POST true  //Power On Self Test (POST). Runs test patterns in setup()


//IDENTIFICATION
char const * NAME  = "SCOREBOARD";          //name or description
char const * MODEL  = "SB001";              //model number
char const * SERIAL_NUMBER  = "000001";     //the unique serial number of this device (6-bytes)
char const * SOFTWARE_VERSION = "0.1";      //numeric: to be changed each rev
char const * SOFTWARE_DATE = __DATE__;      //__DATE__ + 7;
char const * COPYRIGHT_YEAR = __DATE__ + 7;

//BOARD
//MEGA2560 5V

//PROCESSOR
//SAMD51   3.3V


//nRF24L01 (option)
#define NRF24 true

//RASPI
#define RASPI true               //enable Raspberry Pi features
int raspi_getTime_flag = 0;       //1 = get time command pending
int raspi_getTemp_flag = 0;       //1 = get temperature command pending
String raspi_buffer;              //raspi incoming buffer

//SHIELD
//#define SCOREBOARD_PROTOYPE true  //prototype 5V.  (No RTC, EEPROM. No mode switch)
#define PIXELDRIVER false         //PCB: 9-CH, RTC, EEPROM, Mode switch.  5V or 3.3V

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

//PIXEL DRIVER 
#define NEO_LOCAL      49  
Adafruit_NeoPixel neoLocal = Adafruit_NeoPixel(3, NEO_LOCAL, NEO_RGB + NEO_KHZ800);     //3 pixels on-board


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
#define UNINITIALIZED - 1 //not yet 
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
//#define BUZZER      11  //activates buzzer
#define TEMP        12  //temperature
#define TUNER       13  //radio tuner AM/FM
#define TICKER      14  //stock ticker    
#define RANGE       15  //wireless range testing     
#define MODE_MIN   SCOREBOARD
#define MODE_MAX   CLOCK
int mode = CLOCK;    //select one of the above modes
int mode_last = -1;     //remembers the last mode. Used to detect changes or first time through. Default - unitialized 
int flag_test = 0;      //flag for test mode

//COMMAND
//commands are cleared at the end of each loop (run once)

char buffer[256]    = "";
char command[256]   = "";           //this holds the active command   str
char cmdByte        = '\0';         //        "         "         byte.  
char data[256]      = "";
char units[32]      = "";
//char station[32]  = "";
//char stock[8]     = "";           //MSFT, AMZN, GOOG
//char stockval[8]  = "";           //208, 1444
//char stockper[8]  = "";           //percent 1.5  10.1
//char stockdir[8]  = "";           //direction pos(grn), neg(red)


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
int flag_date = 0;                //1=refresh
int flag_timer = 0;                //1=refresh
char current_zulu[32];    
unsigned long timer_sec_base = 0;

//RTC
int flag_rtc = 0;                 //1=detected, 0=not found


//BUZZER relay
#define BUZZER A7                 //MEGA analog pin A7 OUTPUT
#define BUZZER_SEC 3000           //on time. The max number of milliseconds the buzzer is allowed on
bool flag_buzzer = false;         //flag to turn buzzer ON/OFF
unsigned long timerBuzzer;        //number of milliseconds since the buzzer was turned on


//RANGE
int range_c = 0;                  //a generic counter used for testing the RF24L01 wireless remote's range


//PERFORMANCE MONITORS
#define PERFMON false
unsigned long loop_timer = 0; 


/**
 * setup
 */
void setup() {

  //SERIAL
  Serial.begin(19200);            //19.2K is fast as possible.  38.4k has errors.
  delay(1000);
  Serial.print("\nSCOREBOARD ");
  #ifndef PIXELXDRIVER
  Serial.print("MEGA 2560");
  #else
  Serial.print("with PIXEL DRIVER board... "); 
  #endif
  Serial.println("\nBegin setup()");
  

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
  if(NRF24){
    setup_RF24L01();                          //setup nRF24
  }
  
  //RTC
  if(PIXELDRIVER){
    flag_rtc = setup_rtc();                 //detect and setup, I2C device
  }

  //EEPROM
  if(PIXELDRIVER){
   setup_eeprom();                          //detect and setup, I2C device
  }
  
  //RASPI
  if(RASPI){
    setup_raspi();
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

  neoLocal.begin();  //PIXEL DRIVER 3-pixels on-board


  //POWER-ON SELF TEST - run test pattern(s) - also helpful to indicate unexpected reboots
  if(POST) {
    random_beauty();
    //primary_colors();
    //countdown_digit_test();
  }


  //SCOREBOARD
  if(mode == SCOREBOARD){
    scoreboard_init();           //init scoring
    //dt.timer_init();             //init timer
  }
  delay(120);

  //CLOCK DATE TIME 
  if(0){ 
    dt.dtInit();                 //init set date and time
  }

  //TIMERS/COUNTERS
  if(PERFMON){
    loop_timer = millis();        //init
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
  if(NRF24 && !cmdByte) {
    cmdByte = loop_RF24L01();   //always read pending commands from the nRF24L01
  }


  //USB, RasPi
  if(RASPI && !cmdByte){
    //get Raspi command
    cmdByte = loop_raspi();
    //if(cmdByte){Serial.print("Got a RASPI byte: ");Serial.println(cmdByte);}
  }

  //COMMAND STRINGS
  //see "raspi" file
  //
  //$SB<?&cmd=m
  //$RASPI>?&cmd=m

   
    
  //COMMAND BYTE
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
    case 'O' :{ dt.timerOnOff = 1; break;}  //timer on
    case 'o' :{ dt.timerOnOff = 0; break;}  //timer off
    
    //timer and clock reset
    case 'C' : {
        if(mode == DATE || mode == DATE_SET || mode == CLOCK || mode == CLOCK_SET){
          Serial.println("RESETTING DATE/CLOCK: ");
          //timerOnOff = 0;       //set to off    
          dt.initZulu();          //YYYYMMddThhmmssZ
          break;
        }
        
        buttonC_hold_sec++;  //inc this counter each time the 'C' command is detected (about once per second)

        if (buttonC_hold_sec > 1 && buttonC_hold_sec < 3) {
          dt.timerMillisClock = 0; //TIMER_RESET; /*reset timer*/ 
        }
        if ((buttonC_hold_sec > 3 ) /*&& (buttonC_hold_sec & 0x01)*/) {  // more than N sec, then every odd numbered second
          dt.timerMillisClock += ADD_TIME_TO_TIMER;
        }
        
        //limit timer to 90 min MAX
        if (dt.timerMillisClock > 5400000 ) {
          dt.timerMillisClock = 5400000 ;
        }   
        dt.timerOnOff = 0;     //set timer to OFF during reset
        dt.calc_timer2minsec();
        dt.draw_timer();       //refresh timer display after reset or when timer timer is OFF
        Serial.print("TIMER RESET TO: "); Serial.println(dt.timerMillisClock);
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
        if (dt.timerMillisClock) {
          dt.timerOnOff = dt.timerOnOff ^ 1;    //bitwise toggle
        }
        Serial.print("TIMER: ");
        if (dt.timerOnOff) {
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
        dt.loop_clock();      //time
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


  //MODE 
  //detect mode changes, set flags to force refresh
  if(mode != mode_last){        
    Serial.print("MODE: "); Serial.print(mode); Serial.print(" ");
    switch(mode){
      //case NOTHING    : {Serial.print("NOTHING");break;}
      //case STANDBY    : {Serial.print("STANDBY");break;}
      case SCOREBOARD : {Serial.print("SCOREBOARD");flag_scoreboard = 1;break;}
      case CLOCK      : {Serial.print("CLOCK");     flag_clock = 1;break;}
      //case CLOCK_SET  : {Serial.print("CLOCK_SET"); flag_clock = 1;break;}
      case DATE       : {Serial.print("DATE");      flag_date = 1;break;}
      //case DATE_SET   : {Serial.print("DATE_SET");  flag_clock = 1;break;}
      case TIMER      : {Serial.print("TIMER");break;}
      ////case CALENDAR   : {Serial.print("CALENDAR");break;}
      ////case RTC        : {Serial.print("RTC_SET");break;}   
      case TEST       : {Serial.print("TEST");flag_test = 1;break;}
      default         : {Serial.print("INVALID");}
    }//end sw
    Serial.println();
  }//end cmp

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

    loop_rtc();              //display RTC values
    
    if(flag_rtc){
       //TODO use RTC valuse for clock, date and time
    }

    flag_rtc = 0;               //clear
  }

  //CLOCK/DATE
  if(mode == CLOCK_SET || mode == DATE_SET){
    dt.dtInit();                //init the clock/calendar
  }

  //WORK IN PROGRESS DATE TIME TIMEBASE
  //always keep the clock ticking regardless of mode
  bool refresh_dt = dt.loop_dt();


  //CLOCK 
  if((mode == CLOCK) && refresh_dt){
//wcp   dt.loop_clock();             //render clock
  }

  //DATE 
  if((mode == DATE) && refresh_dt){
//wcp    dt.loop_date();             //render date
  }


  //TIMER
  //TIMER TICK - establishes the number of times the timer will update per second 
  if ((mode == SCOREBOARD || mode == TIMER) && dt.timerOnOff && (millis() - dt.timerElapsed) > TIMER_TICK) {

    //LIMIT - check for limit beyond MAX
    if (dt.timerMillisClock > 5400000 ) {
      dt.timerMillisClock = 5400000 ;
    }

    //TICK - decrement the timer's time remaining by one timer tick
    dt.timerMillisClock -= TIMER_TICK;   //subtracts a timer tick (milliseconds) from the time remaining
    dt.timerElapsed = millis();          //reset elapsed time since for the next timer tick
    
    //RENDER - draw the timer upon each tick
    //flag_timer = 1;
    //or
    dt.draw_timer();
    
    //EXPIRED - if timer is on and remaining time is zero, then time just expired. Trigger buzzer
    if (dt.timerMillisClock <= 0 ) {
      Serial.println("TIMER EXPIRED");
      dt.timerOnOff = 0;                 //STOP timer
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


  //DEBUG
  //Serial.print("mode: ");Serial.println(mode);

  //CLEAR
  cmdByte = NULL;             //clear the active command each time through the main loop (so commands only run once)
  strcpy(buffer,"");
  strcpy(data,"");
  strcpy(command,"");
  mode_last = mode;           //detect mode changes


  //LOOP
  if(PERFMON){
    //loop performance monitor
    //Serial.print("loop_timer: ");Serial.print(millis() - loop_timer);Serial.println(" sec");
    loop_timer = millis();   
  }

  //delay(20);//???

}//end main loop
