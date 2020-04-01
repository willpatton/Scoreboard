/**
 * 
 * parse command
 * 
*/
int parseCommands(char str[32]) {

    //PARSE HEADER
    //looking for "$SB<"
    char header[4] = "";
    strncpy(header,str,4);
    if(1){Serial.print("header: ");Serial.println(header);}

    //PARSE BYTE   
    if(strcmp("$SB<",header) == 0){  
      //header matched okay, continue parsing...
      cmdByte = str[4];          //the 4th byte is the command
    }


    //PARSE STRINGS
    
    //PART 1 - parse the command string for parameters
    //example ",enL:63,enR:512";
    
    const char delim[] = "?&";  
    char * token; 
    char * ar[10];  //array of tokens
    byte i = 0;    
    //1st token is the header
    token = strtok(str, delim);  //gets first token
    while( token != NULL ) {
      ar[i] = token;
      //Serial.printf( " %s\n", ar[i] );
      token = strtok(NULL, delim);  //NULL increments to next token
      i++;
    }

    //PART 2: get params and values from the array of tokens
    //2nd token and up...
    char * param = "";
    char * value = "";
    const char delim2[] = "=";
    //for each token in the array, parse the param=value 
    for(int n = 0; n < i; n++)
    { 
      param = strtok(ar[n], delim2);  //gets next token  
      Serial.print("param: ");Serial.print(param);
      value = strtok(NULL, delim2);  //NULL increments to next token
      Serial.print(" value: ");Serial.print(value);
      Serial.println();

      int screen = 0;
      int swPosL = 0;
      int swPosR = 0;
      int enPosL = 0;
      int enPosR = 0;
      int focusL = 0;
      int focusR = 0;
      
      //command
      if(strcmp("cmd",param) == 0){strcpy(command, value);}         //only 1 command
      //screen
      if(strcmp("sc",param) == 0){screen = String(value).toInt();}  //only 1 screen  
      //switches
      if(strcmp("swL",param) == 0){
        swPosL = String(value).toInt();
        mode = swPosL;
        }
      if(strcmp("swR",param) == 0){swPosR = String(value).toInt();}
      //encoders
      if(strcmp("enL",param) == 0){
        Serial.print("enPosL: ");Serial.print(enPosL);
        enPosL = String(value).toInt();
        score_home = enPosL;
        draw_score_home();       
        }
      if(strcmp("enR",param) == 0){
        enPosR = String(value).toInt();
        score_visitors = enPosR;
        draw_score_visitors();  
        }
      //focus
      if(strcmp("fcL",param) == 0){focusL = String(value).toInt();}  
      if(strcmp("fcR",param) == 0){focusR = String(value).toInt();}  
    }

    Serial.print("MSG str:");Serial.print(str);Serial.print(" command: ");Serial.println(command); 
    
}//end fn
