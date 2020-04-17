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
  
  //FORCE RTC
  //force RTC update from computer time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("COMPLETE ");

  return(detected);
}


/**
 * @breif get time from RTC and return a Zulu string
 */
void getRTCZulu(char * strZulu){
  //Serial.print("\nstrZulu: ");
  DateTime now = rtc.now();
  char MM[3];
  sprintf(MM, "%02d", now.month());
  char DD[3];
  sprintf(DD, "%02d", now.day());
  char hh[3];
  sprintf(hh, "%02d", now.hour());
  char mm[3];
  sprintf(mm, "%02d", now.minute());
  char ss[3];
  sprintf(ss, "%02d", now.day()); 
  String str = String(String(now.year()) + String(MM) + String(DD) + String('T'));
  str = String(str + String(hh) + String(mm) + String(ss) + String('Z'));
  //Serial.println(str);
  strcpy(strZulu, str.c_str()); //str.c_str()
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

  
 }
