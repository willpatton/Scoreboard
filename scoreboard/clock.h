/**
 * clock
 * timer, time/clock, date/calendar
  
  @author:   Will Patton
  @url:      http://github.com/willpatton
  @license:  MIT License
  
   Definitions
   * The timebase is based on the millisecond timer in the MCU.
   * A timer is an interval in minutes, seconds.  Count up or down.
   * A clock displays the time or timer.
   * A calendar displays the date.
     
   
      TIMER 
   +-----------+
   |  HH : MM  |
   |           | 
   +-----------+   
   
      CLOCK
   +-----------+
   |  HH : MM  |
   | AM/PM SS  | 
   +-----------+       
   
      DATE
   +-----------+
   |   YYYY    |
   |  MM  DD   | 
   +-----------+  
           
  Timer: mm:ss
  
  Time: HH:mm:ss.SS
  
  Clock: renders time or timers digits
  
  Date: YYYY-MM-dd
  
  Calendar: render date digits
  
  ISO 8601 - https://en.wikipedia.org/wiki/ISO_8601
  Time formats: Zulu
  "YYYY-MM-ddTHH:mm:ss.SSSZ"
  "YYYY-MM-DDThh:mm:ssZ"         //milliseconds  (e.g. "2019-03-13T15:07:18Z")
  "YYYY-MM-DDTHH:MM:SS.nnnnnnZ"  //microseconds
  
  "yyyyMMddTHHmmssZ" date[17] 

 TODO: 
 * Alarm events
 * RTC Real time clock
 * GPS clock
 * Time server
 * Time zones
 * Daylight savings time
  
*/

#ifndef MyDateTime_h
#define MyDateTime_h

//MACRO
#define NUM(off, mult) ((timeString[(off)] - '0') * (mult))

//TIMEBASE - MILLISECONDS
#define TIMER_TICK 1000           //The number of milliseconds in a timer tick. Used to establish a time base (refresh rate). 
                                  //(1000 = TIMER_TICK updates  1Hz once per second)
                                  //( 100 = TIMER_TICK updates 10Hz 10 times per second)

//TODO - put in this class
//TIMER
int timerOnOff = 0;               //indicates if timer is ON or OFF. 1 = timer ON (running), 0 = timer OFF (stopped)
unsigned long timerMillisClock;   //millisecond timer used to keep the SB timer's time remaining
unsigned long timerElapsed = 0;   //millisecond timer to keep track of elapsed time (used by timer)


class MyDateTime {

  private:

    //DATE & TIME
    int year_   = 2019;                 //year  YYYY
    int month_  = 1;                    //month MM
    int day_    = 1;                    //day   DD
    int hours   = 0;                    //time  hh
    int minutes = 0;                    //min   mm
    int seconds = 0;                    //sec   ss

    //CLOCK DIGITS
    //current digits  //last digits         //-1 = uninitialized, used for force refresh of a single digit
    int hour10 = 0;   int hour10_t  = -1;   //hour 10's
    int hour01 = 0;   int hour01_t  = -1;   //hour 1's
    int min10  = 0;   int min10_t   = -1;   //minute 10's
    int min01  = 0;   int min01_t   = -1;   //minute 1's
    int sec10  = 0;   int sec10_t   = -1;   //second 10's
    int sec01  = 0;   int sec01_t   = -1;   //second 1's
    
    //DATE DIGITS
    //current digits  //last digits         //-1 = uninitialized
    int year1000 = 0; int year1000_t  = -1; //yyyy                  
    int year100 = 0;  int year100_t   = -1;
    int year10 = 0;   int year10_t    = -1;
    int year01 = 0;   int year01_t    = -1;
    int month10 = 0;  int month10_t   = -1; //mm
    int month01 = 0;  int month01_t   = -1;
    int day10 = 0;    int day10_t     = -1; //dd
    int day01 = 0;    int day01_t     = -1;

    //TIMER DIGITS
    //current digits        //last digits               //-1 = uninitialized
    int timer_min10  = 0;   int timer_min10_t   = -1;   //minute 10's
    int timer_min01  = 0;   int timer_min01_t   = -1;   //minute 1's
    int timer_sec10  = 0;   int timer_sec10_t   = -1;   //second 10's
    int timer_sec01  = 0;   int timer_sec01_t   = -1;   //second 1's

    //prototypes
    void calc_date();
    void calc_time();

  public:

    //TIME
    unsigned long timer_sec;   //millisecond timer

    MyDateTime();             
    void dtInit();            
    void initZulu(String timeString);
    void loop_date();
    void loop_time();

    //setters
    void set_year();          
    void set_month();
    void set_day();
    void set_hours();
    void set_minutes();

    //timer
    void calc_timer2minsec();
    void timer_init();
    void draw_timer();

};

//TIMER
#define TIMER_MIN_90  5400000;    //5400000 = 90 min * 60 sec * 1000 milliseconds 
#define TIMER_MIN_5  300000;      //300000  =  5 min * 60 sec * 1000 milliseconds 
#define ADD_TIME_TO_TIMER TIMER_MIN_5;  //set timer value to increments of 5 minutes  

//TIMER values upon RESET 
//(uncomment one --> the value most often used. e.g. 20 min for a hockey period)
//#define TIMER_RESET 5400000;      //5400000 = 90 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 3600000;      //3600000 = 60 min * 60 sec * 1000 milliseconds 
  #define TIMER_RESET 1200000;      //1200000 = 20 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 900000;       // 900000 = 15 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 600000;       // 600000 = 10 min * 60 sec * 1000 milliseconds 
//#define TIMER_RESET 300000;       // 300000 =  5 min * 60 sec * 1000 milliseconds
//#define TIMER_RESET 60000;        //  60000 =  1 min * 60 sec * 1000 milliseconds
//#define TIMER_RESET 30000;        //  30000 =  0 min * 30 sec * 1000 milliseconds
//#define TIMER_RESET 20000;        //  20000 =  0 min * 20 sec * 1000 milliseconds

#endif
