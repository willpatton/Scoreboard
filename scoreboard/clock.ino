/**
 * timer, time/clock, date/calendar
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 */

#include "clock.h"


/**
 * constructor
 */
MyDateTime::MyDateTime(){
  
  //nothing yet
  //dtInit();  //cannot use this here because serial ports are not initialized during constructor
  timer_sec_base = (unsigned long)millis(); 
}

/**
 * getters
 */


/**
 * setters
 * Used by handheld remote
 */
void MyDateTime::set_year(){
  year_++;
}
void MyDateTime::set_month(){
  month_++;
}
void MyDateTime::set_day(){
  day_++;
}
void MyDateTime::set_hours(){
  hours++;
}
void MyDateTime::set_minutes(){
  minutes++;
}


/**
 * date and time init
 * 
 * detects if date/time are uninitialized
 * set date and time using zulu format
*/
void MyDateTime::dtInit(){
  
  Serial.println("INIT DATE and TIME");

  //detect uninitialized date
  if(year1000_t < 0){
    //clear_all();
    Serial.println("DATE: Uninitialized date");
  }

  //detect uninitialized clock
  if(hour10_t < 0){
    Serial.println("CLOCK: Uninitialized clock");
  }
  
  /**
  * THIS SETS THE TIME
  */
  initZulu(this->time_zulu);

  //RESET the 1-second interval timer 
  timer_sec_base = (unsigned long)millis();       
}


/**
 * 
 */
void MyDateTime::initZulu(){
  initZulu(this->time_zulu);
}
/**
 * init date and time by Zulu string
 * 
 * ZULU FORMAT EXAMPLE:
 * "yyyyMMddTHHmmssZ" 
 * "20190214T120300Z"
 */
 void MyDateTime::initZulu(String zuluString){
    
  if (zuluString[8] == 'T' && zuluString[15] == 'Z') {
    Serial.print("TIME ZULU INIT: "); Serial.println(zuluString);
    year_ = (zuluString[0] - '0') * 1000 +
    (zuluString[1] - '0') * 100 +
    (zuluString[2] - '0') * 10 +
    (zuluString[3] - '0');
    year_ =   NUM(0, 1000) + NUM(1, 100) + NUM(2, 10) + NUM(3, 1);
    month_ =  NUM(4, 10)   + NUM(5, 1);
    day_ =    NUM(6, 10)   + NUM(7, 1);
    hours =   NUM(9, 10)   + NUM(10, 1);
    minutes = NUM(11, 10)  + NUM(12, 1);
    seconds = NUM(13, 10)  + NUM(14, 1);
  }
  
}


/**
 * loop datetime
 * This keeps the date/time timebase always running
 */

bool MyDateTime::loop_dt(){
  /*
  //run this function every 1 second
  unsigned long diff = millis() - timer_sec;
  if(diff > 1000){
      timer_sec = millis() + diff - 1000; //reset 1-second periodic timer
      return true;
  } 
  return false;
  */
  //run this function every 1 second
  uint32_t diff = (millis() - timer_sec_base);
  if(diff > 1000){
      //debug
      //Serial.print("loop_clock() diff:");Serial.print(diff);Serial.print(" ");
      //Serial.print(" timer_sec_base: ");Serial.print(timer_sec_base);Serial.print(" ");
      //Serial.println();
      timer_sec_base = millis(); 

      //TODO - wcp
      //if(mode == DATE){loop_date();}
      //if(mode == CLOCK){loop_clock();}
      loop_date();
      loop_clock();
      return true;
  }
    
  return false; 
}

/*
 * date
 */
 void MyDateTime::loop_date(){
  /*
  //only run this function every 1 second
  if(millis() - timer_sec < 1000){
    return;
  } 
  timer_sec = millis(); //reset 1-second timer
  */
  //if(!loop_dt()){return;}


  //check limits
  if(1){
    if(month_ > 12){month_ = 1;}        //rollover to 1
    switch(month_){
      case 2 : { 
        //Feb
        if((year_ % 4) == 0){
          if(day_ > 2){day_ = 1;}break; //leap year
          } 
        if(day_ > 28){day_ = 1;} break; //not leap year
        } 
      case 4 : //Apr
      case 6 : //Jun 
      case 9 : //Sep
      case 11 : {if(day_>30){day_ = 1;}break;} //Nov
      default : if(day_>31){day_ = 1;}
    }     
    flag_date = 1; //force refresh
  }

   //force refresh for all of these "date" digits
   if(flag_date){
      year1000_t  = -1;
      year100_t   = -1;
      year10_t    = -1;
      year01_t    = -1;
      month10_t   = -1;
      month01_t   = -1;
      day10_t     = -1;
      day01_t     = -1;
      flag_clock = 0; //clear
   }
    
  //convert date to digits
  calc_date(); 

if(mode == DATE){
  //RENDER only the digits that have changed
  Serial.print(year_);Serial.print("-");Serial.print(month_);Serial.print("-");Serial.print(day_);Serial.println(" ");
  draw(' ', 4);     //colon is digit offset 4.  Be sure colon is off for year
  if(year1000_t != year1000){draw(year1000  + 0x30, 3);}
  if(year100_t  != year100) {draw(year100   + 0x30, 2);}
  if(year10_t   != year10)  {draw(year10    + 0x30, 1);}
  if(year01_t   != year01)  {draw(year01    + 0x30, 0);}  

  if(month10_t != month10){draw(month10  + 0x30, 6);}
  if(month01_t != month01){draw(month01  + 0x30, 5);}
  if(day10_t != day10){draw(day10  + 0x30, 8);}
  if(day01_t != day01){draw(day01  + 0x30, 7);}
}

  //SAVE - existing digit values for next loop
  year1000_t  = year1000;
  year100_t   = year100;
  year10_t    = year10;
  year01_t    = year01;
  month10_t   = month10;
  month01_t   = month01;
  day10_t     = day10;
  day01_t     = day01;

 }


/**
 * time
 */
 void MyDateTime::loop_clock(){
  /*
  //only run this function every 1 second
  if(millis() - timer_sec < 1000){
    return;
  } 
  timer_sec = millis(); //reset 1-second timer
  */
  //if(!loop_dt()){return;}


flag_clock = 1;
   //DIGITS - force refresh for all clock digits
   if(flag_clock){
      
      hour10_t  = -1;
      hour01_t  = -1;
      min10_t   = -1;
      min01_t   = -1;
      sec10_t   = -1;
      sec01_t   = -1;
      flag_clock = 0; //clear

      //TEMP - only draw if flag set
      if(mode == CLOCK){draw(':', 4);} //colon is digit offset 4. 
   }
   
  //convert time to digits
  calc_time(); 

if(mode == CLOCK){
  //render digits in ASCII format (add 0x30 to each numeric value to get ASCII digits) 
  if(hour10_t != hour10){
   if(hour10 == 0) {draw(' ', 3);}     //detect 12am to 1am transition
   if(hour10){draw(hour10 + 0x30, 3);} //only show hour-tens if non-zero
  }
  if(hour01_t != hour01){draw(hour01 + 0x30, 2);}
  if(min10_t != min10){draw(min10  + 0x30, 1);}
  if(min01_t != min01){draw(min01  + 0x30, 0);}
  if(sec10_t != sec10){draw(sec10  + 0x30, 8);}
  if(sec01_t != sec01){draw(sec01  + 0x30, 7);}
  //draw(':', 4); //colon is digit offset 4. 

   
  //render AM/PM 
  char vis10 = 'A';
  if(hours > 11){
  vis10 = 'P';
  }
  char vis01 = 'M';
  draw(vis10, 6);  //render only in CLOCK mode
  draw(vis01, 5);
  
  //DATE & TIME - echo to console
  //Serial.print(year_);Serial.print("-");Serial.print(month_);Serial.print("-");Serial.print(day_);Serial.print(" ");
  Serial.print(hour10);Serial.print(hour01);Serial.print(":");Serial.print(min10);Serial.print(min01);Serial.print(".");Serial.print(sec10);Serial.print(sec01);
  Serial.print(" ");
  Serial.print(vis10);Serial.print(vis01); //AM or PM
  Serial.println();

}//end render
 
  //SAVE - existing digit values for next loop
  hour10_t = hour10;
  hour01_t = hour01;
  min10_t = min10;
  min01_t = min01;
  sec10_t = sec10;
  sec01_t = sec01;
 }


/**
 * calc date
 * convert date to digits
 */
 void MyDateTime::calc_date(){
  //Serial.println("calc_date");
  int r = 0;
  year1000  = year_ / 1000;
  r  = year_ % 1000;
  //Serial.print(year1000); Serial.print(" r "); Serial.println(r); 
  year100 = r / 100;
  r = r % 100;
  //Serial.print(year100); Serial.print(" r "); Serial.println(r); 
  year10  = r / 10;
  year01 = r  = r % 10;
  //Serial.print(year10); Serial.print(" r "); Serial.println(r); 
  //Serial.println(year01);  
  month10 = month_ / 10;
  month01 = month_ - (month10 * 10);
  day10   = day_ / 10;
  day01   = day_ - (day10 * 10);
}


/**
 * calc time
 * convert time to clock digits
 */
 void MyDateTime::calc_time(){
  
  //TICK TOCK, INCREMENT THE CLOCK
//wcp  if(mode == CLOCK){
    //only increment the clock if in CLOCK mode, skip for CLOCK_SET mode
    seconds++;
//wcp  }
  
  if(seconds > 59){
    seconds = 0;
    minutes++;
  }
  if(minutes > 59){
    minutes = 0;
    hours++;
  }
  if(hours > 23){
    day_++;  
    hours = 0;
    minutes = 0;
    seconds = 0;
    
    //TODO - MIDNIGHT - re-sync time at midnight-
    //time_sync(); 
  }

  //CONVERT TIME TO CLOCK DIGITS
  int hour_t = hours; //temp
  if(hours > 12){
    hour_t -= 12;
  }
  if(hour_t <= 12){
    hour10 = hour_t / 10; //HH
    hour01 = hour_t - (hour10 * 10);
  }
  if(hours == 0){
    hour10 = 1;
    hour01 = 2;
  }
  min10 = minutes /10;    //mm
  min01 = minutes - (min10 * 10);
  sec10 = seconds / 10;    //ss
  sec01 = seconds - (sec10 * 10);
  
 }


/**
 * timer
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * Timer: coundown or count up
 * 
 * Time base: 1 millisecond resolution
 * 
 * Clock: mm:ss   0 to 99:59
 *  
 *  
 */


/**
 * timer init
 */
void MyDateTime::timer_init(){
  
    dt.timerMillisClock = TIMER_RESET; //reset the clock's main timer. Holds the time remaining in milliseconds (e.g. 900000 milliseconds = 15 minutes)
    draw_timer();                   //render timer's digits to setup() values
    dt.timerElapsed = millis();        //init clock's timer
    timerBuzzer = millis();         //init clock's buzzer
}


/**
 * draw timer - render the timer's digits
 */
void MyDateTime::draw_timer(){

   //clear all timer's pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digMin10.setPixelColor(i,0);
    digMin01.setPixelColor(i,0);
    digSec10.setPixelColor(i,0);
    digSec01.setPixelColor(i,0);
   }

   //convert/update the timers's millisecond timer to min:sec
   calc_timer2minsec();
   Serial.print("TIMER: ");Serial.print(min10);Serial.print(min01);Serial.print(":");Serial.print(sec10);Serial.print(sec01);Serial.println();

   //render clock digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   if(min10 == 0){draw(' ', 3);}    //clear the min 10's column using an ASCII space to act as a digit clear
   if(min10 > 0){draw(min10 + 0x30, 3);}
   if(min01 == 0){draw(' ', 2);}    //clear the min 01's column using an ASCII space to act as a digit clear
   if(min01 > 0 || min10 > 0){draw(min01 + 0x30, 2);}
   draw(sec10 + 0x30, 1);
   draw(sec01 + 0x30, 0);
   draw(':', 4);    //colon is digit offset 4.  Don't forget to illuminate the colon, too, here!
}


/**
 * timer2minsec - convert/update the clock's millisecond timer to min:sec
 */
void MyDateTime::calc_timer2minsec(){

   int minutes = dt.timerMillisClock / 60000;
   min10 = (minutes / 10);
   min01 = minutes - (min10 * 10);
   unsigned long milliseconds = dt.timerMillisClock - (minutes * 60000);
   sec10 = milliseconds / 10000;
   sec01 = (milliseconds / 1000) - (sec10 * 10); 

}
