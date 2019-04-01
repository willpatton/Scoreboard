/**
 * timer
 * 
 * A countdown timer. 
 * For use to countdown 20 minute for a hockey period.
 *
 * 
 * AUTHOR: Will Patton. http://willpatton.com
 * 
 * Note: The concept of the "time base", "timer clock", and the "clock face" are all somewhat different.
 * The time base is the interval (in milliseconds) when the time keeping updates (e.g. 1000 = update each 1 second).
 * The timer clock is the number of milliseconds remaining.  (900000 milliseonds is 15 min, 0 sec.  Or, 15:00 on the clock face).
 * The clock face is representation of the milliseconds remaining formated as "min:sec".  
 * 
 */

/**
 * 
 * timer2minsec - convert/update the clock's millisecond timer to min:sec
 * 
 */
void timer2minsec(){

   int minutes = timerMillisClock / 60000;
   min10 = (minutes / 10);
   min01 = minutes - (min10 * 10);
   unsigned long milliseconds = timerMillisClock - (minutes * 60000);
   sec10 = milliseconds / 10000;
   sec01 = (milliseconds / 1000) - (sec10 * 10); 

   //Also show values on the Serial console
   Serial.print("TIMER: ");Serial.print(min10);Serial.print(min01);Serial.print(":");Serial.print(sec10);Serial.print(sec01);Serial.println();
}


/**
 * draw timer - render the timer's digits
 */
void draw_timer(){

   //clear all timer's pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digMin10.setPixelColor(i, 0);
    digMin01.setPixelColor(i, 0);
    digSec10.setPixelColor(i, 0);
    digSec01.setPixelColor(i, 0);
   }

   //convert/update the timers's millisecond timer to min:sec
   timer2minsec();
  
   //render clock digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   if(min10 == 0){draw(' ', 3);}    //clear the min 10's column using an ASCII space to act as a digit clear
   if(min10 > 0){draw(min10 + 0x30, 3);}
   if(min01 == 0){draw(' ', 2);}    //clear the min 01's column using an ASCII space to act as a digit clear
   if(min01 > 0 || min10 > 0){draw(min01 + 0x30, 2);}
   draw(sec10 + 0x30, 1);
   draw(sec01 + 0x30, 0);
   draw(':', 4);    //colon is digit offset 4.  Don't forget to illuminate the colon, too, here!
}

