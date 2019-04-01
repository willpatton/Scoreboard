/**
 * clock
 * 
 * A device for measuring time.
 * 
 * 12-hour AM/PM
 * 
 * HOURS : MIN
 *    12 : 03
 * SECS  :   
 *    00   AM/PM
 *         
 *         
 * Preferred time format: Zulu     
 * "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'" 
 * 
 */

//MACRO
#define NUM(off, mult) ((timeString[(off)] - '0') * (mult))


//LAST VALUES - last digit values
int hour10_t = -1;
int hour01_t = -1;
int min10_t = -1;
int min01_t = -1;
int sec10_t = -1;
int sec01_t = -1;

  

/**
 * setup  
 */
 void setup_time(String timeString){

  //INIT - set values gotten from time server, GPS, or remote link 
  //"yyyyMMddTHHmmssSSSZ" 
  //String timeString = "20190214T120300000Z";
  if (timeString[8] == 'T' && timeString[15] == 'Z') {
    Serial.println("TIME INIT:");
    year_ = (timeString[0] - '0') * 1000 +
    (timeString[1] - '0') * 100 +
    (timeString[2] - '0') * 10 +
    (timeString[3] - '0');
    year_ =   NUM(0, 1000) + NUM(1, 100) + NUM(2, 10) + NUM(3, 1);
    month_ =  NUM(4, 10)   + NUM(5, 1);
    day_ =    NUM(6, 10)   + NUM(7, 1);
    hours =   NUM(9, 10)   + NUM(10, 1);
    minutes = NUM(11, 10)  + NUM(12, 1);
    seconds = NUM(13, 10)  + NUM(14, 1);
  }
  
 }


/**
 * loop
 */
 void loop_time_zulu(){

  //only run this function every 1 second
  if(millis() - timerTimeZulu < 1000){
    return;
  }
  timerTimeZulu = millis(); //reset second timer

  //Uninitialized clock
  //detect 1st time through, clear display
  if(hour10_t < 0){
    clear_all();
    Serial.println("CLOCK: Uninitialized clock");
    Serial.println("MODE: Setting mode to CLOCK_SET");
    mode = CLOCK_SET;
  }

  //CALC - Update time each second
  calc_time();

  //render clock digits in ASCII format (add 0x30 to each numeric value to get ASCII digits) 
  if(hour10_t != hour10){
   if(hour10 == 0) {draw(' ', 3);}     //detect 12am to 1am transition
   if(hour10){draw(hour10 + 0x30, 3);} //only show hour-tens if non-zero
  }
  if(hour01_t != hour01){draw(hour01 + 0x30, 2);}
  if(min10_t != min10){draw(min10  + 0x30, 1);}
  if(min01_t != min01){draw(min01  + 0x30, 0);}
  if(sec10_t != sec10){draw(sec10  + 0x30, 8);}
  if(sec01_t != sec01){draw(sec01  + 0x30, 7);}
  draw(':', 4); //colon is digit offset 4. 
  
  //render AM/PM 
  char vis10 = 'A';
  if(hours > 11){
  vis10 = 'P';
  }
  char vis01 = 'M';
  draw(vis10, 6);
  draw(vis01, 5);
  
  //display too!
  /*Serial.print("DATE:");*/Serial.print(year_);Serial.print("-");Serial.print(month_);Serial.print("-");Serial.print(day_);Serial.print(" ");
  /*Serial.print("TIME: ");*/Serial.print(hour10);Serial.print(hour01);Serial.print(":");Serial.print(min10);Serial.print(min01);Serial.print(".");Serial.print(sec10);Serial.print(sec01);
  Serial.print(" ");
  Serial.print(vis10);Serial.print(vis01);
  Serial.println();

  //SAVE - existing digit values for next loop
  hour10_t = hour10;
  hour01_t = hour01;
  min10_t = min10;
  min01_t = min01;
  sec10_t = sec10;
  sec01_t = sec01;
 }


/**
 * 
 */
 /*
void loop_time_date_zulu(){
  
}
*/

/**
 * calc time every timer tick (1-second)
 */
 void calc_time(){
  

  //TICK TOCK, INCREMENT THE CLOCK
  if(mode == CLOCK){
    seconds++;
  }
  
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
    //setup_time(); 
  }

  //CLOCK TO DIGITS
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

