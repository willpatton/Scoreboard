/** 
 * RTC real time clock
 *  
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 *  
 *  Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 *  
 *  I2C Address: ________
 */
#include <Wire.h>
#include "RTClib.h"

RTC_PCF8523 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

bool setup_rtc () {

  bool detected = false;

  Serial.print("Setup RTC: ");

  //begin
  if (detected = rtc.begin()) {
    Serial.print("FOUND "); 
  }

  //INIT - initialize the RTC upon 1st time use
  if (!rtc.initialized()) {
    Serial.print("First time initialization. ");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));  
  }

  Serial.println("COMPLETE ");

  return(detected);
}


/**
 * loop rtc - get rtc
 * 
 * run this once per day at midnight to synchronize the 
 *  clock/calendar to the RTC
 */
 void loop_rtc(){ 

  //if(!rtc.initialized()){
  //  return;
  //}
  
  DateTime now = rtc.now();

  //TODO - wcp moved to class
  /*
  year_ = now.year();
  month_ = now.month();
  day_ = now.day();
  hours = now.hour();
  minutes = now.minute();
  seconds = now.second();
  */
  
 }
