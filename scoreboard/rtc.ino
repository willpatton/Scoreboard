// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_PCF8523 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup_rtc () {

  Serial.print("RTC setup: ");

  if (!rtc.begin()) {
    Serial.println("RTC: NOT FOUND");
    //while (1);
  }

  if (!rtc.initialized()) {
    Serial.println("RTC: First time initialization. ");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

 //get RTC now
 loop_rtc();

 Serial.println("Complete");
}


/**
 * Loop
 */
 void loop_rtc(){ 

  //if(!rtc.initialized()){
  //  return;
  //}
  
  DateTime now = rtc.now();
  
  year_ = now.year();
  month_ = now.month();
  day_ = now.day();
  hours = now.hour();
  minutes = now.minute();
  seconds = now.second();
  
 }

