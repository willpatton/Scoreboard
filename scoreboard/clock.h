/**
 * 
 * date and time  - the timebase
 * clock and calendar - the rendering of digits
 * 
 */
#ifndef MyDateTime_h
#define MyDateTime_h

class MyDateTime {
  
  private:

    //DATE & TIME 
    int year_ = 0;                   //year  yyyy
    int month_ = 0;                  //month mm
    int day_ = 0;                    //day   dd
    int hours = 0;                  //time
    int minutes = 0;
    int seconds = 0;
    
    //CLOCK - digits
    int hour10_t = -1;
    int hour01_t = -1;
    int min10_t = -1;
    int min01_t = -1;
    int sec10_t = -1;
    int sec01_t = -1;
    
    void calc_time();
  
  public:
    //TIME
    unsigned long TimeZulu;
    
    MyDateTime(); //constructor
    void setup_time(String timeString);
    void loop_time_zulu();
    void clock_init();
    void set_hours(); 
    void set_minutes();
  
};

#endif
