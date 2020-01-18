/**
* raspi
*
* A plethora of Raspberry Pi features due to connectivity
*
*
*/

#ifdef RASPI

/*
* setup
*/
void setup_raspi(){
    Serial.println("$SB>msg=Scoreboard");    //send msg
    Serial.print("$SB>ver="); Serial.println(SOFTWARE_VERSION);    //send msg
}  
 
 
/*
* loop
*/
void loop_raspi(){
   
 /* 
  char buff;//[257] = "\0";
  while(Serial.available()){
    //READ by strin
    buff = Serial.read();   //read byte
    //buffer.trim();                         //remove any stray NL or CR
    if(1){Serial.print(buff);}   
  }
 */ 
  
  //String buffer;
  
  //is there an incoming msg?
  if(Serial.available()){
    //READ by string
    String buffer = Serial.readString();   //read incoming serial stream 
    buffer.trim();                         //remove any stray NL or CR
    if(1){/*Serial.print("Msg rcvd: "); */ Serial.println(buffer);}   
  }
  
  
}

/**
* time
*/
void raspi_getTime(){ 
  //get time from Raspi
  Serial.print("$SB>cmd=gettime");    //send get time msg
  raspi_getTime_flag = true;
}

/**
* temperature
*/
void raspi_getTemp(){ 
  //get time from Raspi
  Serial.print("$SB>cmd=gettemp");    //send get temp msg
  raspi_getTemp_flag = true;
}


#endif
