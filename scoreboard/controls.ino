/*
 * controls 
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * read the buttons, switches by polling
 */


/**
 * loop
 */
 void loop_controls(){

  /**
     BUTTONS - poll buttons on circuit board
     Used for development (or without controller)
  */
  //MODE - mode
  if (digitalRead(BUTTON_MODE) == LOW && buttonM == HIGH) { //detect press
    buttonM = LOW;
    cmdByte = 'm';      //mode 
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_MODE) == HIGH && buttonM == LOW) { //detect release
    buttonM = HIGH;
    buttonM_hold = 0;
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_MODE) == LOW && buttonM == LOW) { //detect hold
    buttonM_hold++;
    delay(5); //slow down hold sampling
    if (buttonM_hold > 1000) {
      buttonM_hold = 0;
      cmdByte = 'M';
    }
  }
  
  //HOME - score button, hours button
  if (digitalRead(BUTTON_HOME) == LOW && buttonH == HIGH) { //detect press
    buttonH = LOW;
    cmdByte = 'h';      //home score 
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_HOME) == HIGH && buttonH == LOW) { //detect release
    buttonH = HIGH;
    buttonH_hold = 0;
    delay(10);          //debounce
  }
  if (digitalRead(BUTTON_HOME) == LOW && buttonH == LOW) { //detect hold
    buttonH_hold++;
    delay(5); //slow down hold sampling
    if (buttonH_hold > 1000) {
      buttonH_hold = 0;
 //     score_home = -1; //this will reset the score to zero when the command runs below
      cmdByte = 'H';
    }
  }
  
  //VISITORS - score button
  if (digitalRead(BUTTON_VISITOR) == LOW && buttonV == HIGH) { //detect press
    buttonV = LOW;
    cmdByte = 'v';    //vistor's score 
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_VISITOR) == HIGH && buttonV == LOW) { //detect release
    buttonV = HIGH;
    buttonV_hold = 0;
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_VISITOR) == LOW && buttonV == LOW) { //detect hold
    buttonV_hold++;
    delay(5); //slow down hold sampling
    if (buttonV_hold > 1000) {
      buttonV_hold = 0;
//      score_visitors = -1; //this will reset the score to zero when the command runs below
      cmdByte = 'V';
    }
  }
  
  //TIMER - button
  if (digitalRead(BUTTON_TIMER) == LOW && buttonC == HIGH) { //detect press
    buttonC = LOW;
    cmdByte = 'c';    //toggle timer on/off
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_TIMER) == HIGH && buttonC == LOW) { //detect release
    buttonC = HIGH;
    buttonC_hold = 0;
    delay(10);        //debounce
  }
  if (digitalRead(BUTTON_TIMER) == LOW && buttonC == LOW) { //detect hold
    buttonC_hold++;
    delay(2);         //slow down hold sampling
    if (buttonC_hold > 1000) {
      buttonC_hold = 0;
      cmdByte = 'C';  // 'C'lear timer
    }
  }
  
  //MULTIPLE BUTTONS (TOP 2-BUTTONS plus HOLD)
  if (digitalRead(BUTTON_HOME) == LOW && digitalRead(BUTTON_VISITOR) == LOW) {
    buttonH_hold++;
    buttonV_hold++;
    if(buttonH_hold >300 && buttonV_hold > 300){
      //Serial.println("MODE change");
      cmdByte = 'm';    // change mode
      buttonH_hold = 0; //clear hold counter
      buttonV_hold = 0;
    }   
  }
  
  //MULTIPLE BUTTONS (ALL 3-BUTTONS)
  if (digitalRead(BUTTON_VISITOR) == LOW && digitalRead(BUTTON_TIMER) == LOW && digitalRead(BUTTON_HOME) == LOW) {
    cmdByte = 't';    // 't' run test pattern
  }

  
}//end fn
