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
    Serial.println("$SB>?&cmd=setup");    //send msg
    Serial.println("$SB>?&msg=Hello Scoreboard");    //send msg
    Serial.print("$SB>?&ver="); Serial.println(SOFTWARE_VERSION);    //send ver
}  
 
 
/*
* loop
*/
uint8_t loop_raspi(){

  uint8_t raspiCmdByte = NULL;                  //a character that represents the command
  int debug_ = true;

  //CLEAR
  //strcpy(buffer,"");
  //strcpy(command,"");
  
  if(Serial.available()){
    
      String incoming = Serial.readStringUntil('\n');
      incoming.trim();
      if(incoming.length() < 1){return;}
      if(incoming.length() >= sizeof(buffer)){return;}
      strncpy(buffer,incoming.c_str(),sizeof(buffer));
      if(debug_){
        Serial.print("\nbuffer: "); Serial.print(buffer); Serial.print(" bytes: "); Serial.println(strlen(buffer)); 
      }

      //HEADER
      char header[10];
      strncpy(header,buffer,sizeof(header)-1);
      header[sizeof(header)-1] = '\0';          //force NULL

      //debug
      if(debug_){
        Serial.print("header: "); Serial.print(header); Serial.print(" bytes: "); Serial.println(strlen(header)); 
      }

      //PARSE
      if(strncmp("$RASPI>?&",header,sizeof(header)-1) == 0){
          
        char str[sizeof(buffer)];
        strncpy(str,buffer,sizeof(buffer)-1);
        const char delim[] = "?&";
        char * token;
        char * ar[10];    //array of 10 tokens
        char * param = "";
        char * value = "";
  
        //PART 1 get params
        //1st TOKEN is the header
        token = strtok(str, delim);
  
        //2nd token and up
        byte i = 0;
        while (token != NULL){
          ar[i] = token;
          token = strtok(NULL, delim);
          i++;
        }
        
        //PART2: get values
        const char delim2[] = "=";
  
        //parse each token in the array
        for(int n = 0; n < i; n++){
          param = strtok(ar[n], delim2);  //get next token
          value = strtok(NULL, delim2);   //NULL increments to next token
          
          //debug
          if(debug_){
            Serial.print("param: "); Serial.print(param);
            Serial.print(" value: "); Serial.println(value);
          }
  
          //COMMAND STRING
          if(strcmp("cmd", param) == 0){strcpy(command, value); /*Serial.println(command);*/}
          if(strcmp("value", param) == 0){strcpy(data, value);}
          if(strcmp("units", param) == 0){strcpy(units, value);}

          //COMMAND BYTE
          if(strlen(command) == 1){raspiCmdByte = command[0]; /*Serial.println("GOT A cmdByte");*/}

          //INIT ZULU!!!        
          if(strcmp(command,"setzulu") == 0){
              Serial.println("CMD: SETTING ZULU");
              strcpy(current_zulu, value);
              dt.initZulu(current_zulu);
            }
                  
        }//end for
        
      }//end parse
     
  }//end serial avail

  //cmdBYTE
  return raspiCmdByte;
  
}//end fn


/**
* time
*/
void raspi_getTime(){ 
  //get time zulu from Raspi
  Serial.print("$SB>cmd=getzulu");    //send get time zulu msg
  raspi_getTime_flag = true;
}


/**
* temperature
*/
void raspi_getTemp(){ 
  //get temp from Raspi
  Serial.print("$SB>cmd=gettemp");    //send get temp msg
  raspi_getTemp_flag = true;
}


#endif
