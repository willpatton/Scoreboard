/**
 * draw
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * Draw/render ASCII characters to a wiring harness of 35 indexable RGB LEDs that form a 
 * pattern of pixels as "digit" or font.
 * 
 * The harness of LEDS is arranged into a 5x7 grid that corresponds to a 5x7 font (see fonts.h). 
 * There is helper routine that transposes the layout position of the LEDs to match the grid font. 
 *
 * ASCII characters are sent to the draw() function that illuminate the LEDs in a pattern as a font.
 * 
 * Unkown ASCII characters are echoed to the serial console.
 * 
 * Some fonts like the 1x4 colon font may be passed though here (but it's pattern is not reversed).
 * 
 * 
 * DIGIT ARRANGEMENT:
 * Each digit consists of 35 indexable RGB LEDs.
 * 
 * The digit within the clock's face. 0 is the top-right most digit (e.g. The seconds 1's column)
 *   0 = clock digit sec01 right most 5x7 = 35 pixels
 *   1 = clock digit sec10 
 *   2 = clock digit min01 
 *   3 = clock digit min10 left most
 *   4 = clock colon 1x4 = 4 pixels.  Pixel 0 is top pixel 
 *   5 = score home 1's right most
 *   6 = score home 10's 
 *   7 = score visitors 1's right most
 *   8 = score visitors 10's 
 * 
 * +---------------+
 * |  3  2 4 1  0  |        //digit 4 is the colon (2 pixels)
 * |               |        
 * |               |
 * |  6 5     8 7  |
 * +---------------+
 *      
 *      TIMER
 * +---------------+
 * |   3 2 4 1 0   |        //digit 4 is the colon
 * |   m m : s s   | 
 * +---------------+ 
 *  
 *      SCORE
 * +---------------+
 * |  HOME   VIS   |
 * |  h h    v v   |  
 * +---------------+
 * 
 *      CLOCK
 * +---------------+
 * |  3  2 4 1  0  |        //digit 4 is the colon (2 pixels)
 * |  h  h : m  m  |        
 * |               |
 * |  6 5     8 7  |
 * |  A M     s s  |  
 * +---------------+
 * 
 *       DATE
 * +---------------+
 * |  3  2 4 1  0  |        //digit 4 is the colon (2 pixels) - shown off
 * |  Y  Y   Y  Y  |        
 * |               |
 * |  6 5     8 7  |
 * |  M M     D D  |  
 * +---------------+ 
 * 
 */


/** 
 *  draw ASCII characters to a digit (an array of pixels)
 *  
 *  ch: An ASCII character. The ASCII 'ch'aracter to render
 *  
 *  digits: what digit to draw 0 to 8
 *   
 *  return:  -1 if character not found
 */
void draw(char ch, int digit){

  //default w x h
  int width = 5;        //default width  (for 5x7 fonts)
  int height = 7;       //default height (for 5x7 fonts)

   //special chars/fonts
   if(ch == ':'){
      int width = 1;   
      int height = 4;       
   }

  //calculate number of pixels for the incoming char
  int numpixels = width * height;


  //working buffers
  char ar[numpixels];   //local buffer - used as a local copy of the font
  char c[numpixels];    //destination buffer - used to hold a "rendered" font 
                        //after manipulating it's pixels to match the physical layout of the LEDs

  //clear pixels
  if(digit == 0){digSec01.clear();}
  if(digit == 1){digSec10.clear();}
  if(digit == 2){digMin01.clear();}
  if(digit == 3){digMin10.clear();}
  if(digit == 4){digColon.clear();}
  if(digit == 5){digHom01.clear();}
  if(digit == 6){digHom10.clear();}
  if(digit == 7){digVis01.clear();}
  if(digit == 8){digVis10.clear();}
  

  //Handle each incoming ASCII char and copy it's font to a local working copy called "ar[]" 
  //This table should be sorted by ASCII
  switch(ch) {
    case '0' : {for(int i = 0; i < numpixels; i++){ar[i] = zero[i];} break;}
    case '1' : {for(int i = 0; i < numpixels; i++){ar[i] = one[i];} break;}
    case '2' : {for(int i = 0; i < numpixels; i++){ar[i] = two[i];} break;}
    case '3' : {for(int i = 0; i < numpixels; i++){ar[i] = three[i];} break;}
    case '4' : {for(int i = 0; i < numpixels; i++){ar[i] = four[i];} break;}
    case '5' : {for(int i = 0; i < numpixels; i++){ar[i] = five[i];} break;}
    case '6' : {for(int i = 0; i < numpixels; i++){ar[i] = six[i];} break;}
    case '7' : {for(int i = 0; i < numpixels; i++){ar[i] = seven[i];} break;}
    case '8' : {for(int i = 0; i < numpixels; i++){ar[i] = eight[i];} break;}
    case '9' : {for(int i = 0; i < numpixels; i++){ar[i] = nine[i];} break;}
    case ':' : {for(int i = 0; i < numpixels; i++){ar[i] = colon[i];} break;} //4 pixels in the 1x4 colon font
    case ' ' : {for(int i = 0; i < numpixels; i++){ar[i] = space[i];} break;} 
    case 'A' : {for(int i = 0; i < numpixels; i++){ar[i] = A_A[i];} break;}
    case 'B' : {for(int i = 0; i < numpixels; i++){ar[i] = B_B[i];} break;}
    case 'C' : {for(int i = 0; i < numpixels; i++){ar[i] = C_C[i];} break;}
    case 'D' : {for(int i = 0; i < numpixels; i++){ar[i] = D_D[i];} break;}
    case 'P' : {for(int i = 0; i < numpixels; i++){ar[i] = P_P[i];} break;}
    case 'M' : {for(int i = 0; i < numpixels; i++){ar[i] = M_M[i];} break;}
    //TODO: continue for remaining uppercase letters
    case 'a' : {for(int i = 0; i < numpixels; i++){ar[i] = _a[i];} break;}
    case 'b' : {for(int i = 0; i < numpixels; i++){ar[i] = _b[i];} break;}
    case 'c' : {for(int i = 0; i < numpixels; i++){ar[i] = _c[i];} break;}
    case 'd' : {for(int i = 0; i < numpixels; i++){ar[i] = _d[i];} break;}
    //TODO: continue for remaining lowercase letters
    //Skip missing char's
    //TODO: continue for all remaining ASCII characters
    default : {if(debug){Serial.print("Char not found: "); Serial.print(ch);Serial.print(" ");Serial.println(ch, HEX);} return; /*return(-1);*/}
 
  }

  /**
   * FONT FIXUP
   * This section of code reverses the "odd" rows of a font to match the 
   * physical layout of the LED's bulbs along a string of wires that have 
   * been installed back and forth in a panel.
   * 
   * Run for chars that need to be reversed (e.g. 5x7 fonts of 35 pixels). (e.g. NOT the colon).
   */
   if(1 && numpixels == NUMPIXELS){
     
     bool debug_draw = false ; //true or false. When "true", the font is echoed to the console for debugging
     int i = 0;               //points to source in "ar[]" array
     int idx = 4;             //points to destination in "c[]" array
     int row = 1;             //row counter from 1 to height of pixels "7" for 5x7 fonts

     if(debug_draw){Serial.print("ch: ");Serial.println(ch);}
       
     for(int j = 1; i < numpixels; j++){    
      //even row - just copy the pixel one-to-one
      if(row % 2){
        c[j-1] = ar[i];
        if(debug_draw){Serial.print(ar[i],DEC);Serial.print('.');}    // '.' indicates even rows
      }  
      //odd row - reverse pixels in this "odd" row
      if(!(row % 2)){
        c[j-1] = ar[idx];
        //Serial.print("idx:");Serial.print(idx);Serial.print(" ");
        if(debug_draw){Serial.print(ar[j-1],DEC);Serial.print(',');}  // '.' indicates odd rows
        idx--;
      }
      //find every nth pixel, then adjust indices for the next iteration
      if(!(j % width)){
        if(debug_draw){Serial.println();}
        row++;
        idx = j+4;
      }  
      //increment index to the next pixel
      i++;
     }
     //if(debug_draw){Serial.println();}
   }//end font fixup

  //RENDER
  int brt = BRIGHTNESS;

  for(int i = 0; i < numpixels; i++){
    //clock
    if(c[i] && digit == 0){digSec01.setPixelColor(i, brt,0,0);} //RED
    if(c[i] && digit == 1){digSec10.setPixelColor(i, brt,0,0);} //RED
    if(c[i] && digit == 2){digMin01.setPixelColor(i, brt,0,0);} //RED
    if(c[i] && digit == 3){digMin10.setPixelColor(i, brt,0,0);} //RED
    if(c[i] && digit == 4){digColon.setPixelColor(i, brt,0,0); if(i > 3){break;}} //RED colon
    //score
    if(c[i] && digit == 5){digHom01.setPixelColor(i, brt,brt,brt);} //WHITE
    if(c[i] && digit == 6){digHom10.setPixelColor(i, brt,brt,brt);} //WHITE
    if(c[i] && digit == 7){digVis01.setPixelColor(i, brt,brt,brt);} //WHITE
    if(c[i] && digit == 8){digVis10.setPixelColor(i, brt,brt,brt);} //WHITE
  }
  //clock
  if(digit == 0){digSec01.show();}
  if(digit == 1){digSec10.show();}
  if(digit == 2){digMin01.show();}
  if(digit == 3){digMin10.show();}
  if(digit == 4){digColon.show();}  //colon
  //score
  if(digit == 5){digHom01.show();}
  if(digit == 6){digHom10.show();}
  if(digit == 7){digVis01.show();}
  if(digit == 8){digVis10.show();}
  
}//end draw()


/**
 * random beauty
 * Fills all the pixels with random colors.
 * Uses a local timer.
 * Useful to check update rate and color palette.
 */
void random_beauty(){

  int brt = BRIGHTNESS;
  
  //create local timer (e.g. 1400 milliseconds)
  int timer = millis();
  unsigned val = 0;
  while(val < 1400) {
    for(int i = 0; i < NUMPIXELS; i++){
      //clock
      digSec01.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt));   
      digSec10.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      digMin01.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt));  
      digMin10.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      digColon.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      //scores
      digHom01.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      digHom10.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      digVis01.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      digVis10.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt)); 
      //onboard local
      neoLocal.setPixelColor(i, random(0,brt), random(0,brt), random(0,brt));
            
      //render
      digSec01.show(); //clock
      digSec10.show();
      digMin01.show();
      digMin10.show();
      digColon.show();
      digHom01.show(); //scores
      digHom10.show();
      digVis01.show();
      digVis10.show();
      neoLocal.show();
    }
    //calculate elapsed time
    val = millis() - timer;
  }
  clear_all(); //clear pixels when finished
}


/**
 * primary color pattern
 * Useful to measure current draw.
 */
void primary_colors(){

   int brt = BRIGHTNESS;
   
   int r=0, g=0, b = 0;
   for(int n = 0; n < 4; n++){
     //set colors to r, g, b, w
     switch(n){
      case 0 : {r=brt; g=0;   b = 0;    break;} //r
      case 1 : {r=0;   g=brt; b = 0;    break;} //g
      case 2 : {r=0;   g=0;   b = brt;  break;} //b
      case 3 : {r=brt; g=brt; b = brt;  break;} //w
     }
  
    for(int i=0; i< NUMPIXELS; i++){
      digSec01.setPixelColor(i, r, g, b); digSec01.show(); //clock digit second 1's
      digSec10.setPixelColor(i, r, g, b); digSec10.show(); //10's
      digMin01.setPixelColor(i, r, g, b); digMin01.show(); //clock digit minute 1's
      digMin10.setPixelColor(i, r, g, b); digMin10.show(); //10's
      digColon.setPixelColor(i, r, g, b); digColon.show(); //colon
      digHom01.setPixelColor(i, r, g, b); digHom01.show(); //score home 1's
      digHom10.setPixelColor(i, r, g, b); digHom10.show(); //10's
      digVis01.setPixelColor(i, r, g, b); digVis01.show(); //score visitor 1's
      digVis10.setPixelColor(i, r, g, b); digVis10.show(); //10's
      neoLocal.setPixelColor(i, r, g, b); neoLocal.show(); //
    }
    delay(700);
   }
   clear_all(); //clear pixels when finished
}


/**
 * countdown_digit_test
 * Useful to check fonts.
 */
void countdown_digit_test(){
    
  //numerals
  for(char d='9'; d >='0'; d--){
    clear_all();
    draw(d, 0); //clock seconds 1's
    draw(d, 1); //clock 10 seconds 
    draw(d, 2); //clock minutes 1's
    draw(d, 3); //clock 10 minutes 
    //draw(d, 4); //colon is NOT a digit - this comment is a non-sense placeholder for the colon pixels ;)
    draw(d, 5); //score home 1's
    draw(d, 6); //score home 10'
    draw(d, 7); //score vistors 1's
    draw(d, 8); //score visitor 10's 
    delay(700); 
  }
  //uppercase letters - scroll
  for(char d='A'; d <='D'; d++){
    clear_all();
    draw(d+3, 0); //clock - left most digit
    draw(d+2, 1); //
    draw(d+1, 2); //
    draw(d, 3);   //clock - right most digit
    delay(700); 
  }
  //lowercase letters - scroll
  for(char d='a'; d <='d'; d++){
    clear_all();
    draw(d+3, 0); //clock - left most digit
    draw(d+2, 1); 
    draw(d+1, 2); 
    draw(d, 3);   //clock - right most digit
    delay(700); 
  }
  clear_all(); //clear pixels when finished
}


/**
   clear all pixels to off "0", everywhere
*/
void clear_all() {
  Serial.println("CLEAR - all pixels");
  
  //clear all pixel memory
  for (int i = 0 ; i < NUMPIXELS; i++) {
    digSec01.setPixelColor(i, 0);
    digSec10.setPixelColor(i, 0);
    digMin01.setPixelColor(i, 0);
    digMin10.setPixelColor(i, 0);
    digColon.setPixelColor(i, 0);  //TODO: Only 4 pixels in the the colon.  Not 35.
    digHom01.setPixelColor(i, 0);
    digHom10.setPixelColor(i, 0);
    digVis01.setPixelColor(i, 0);
    digVis10.setPixelColor(i, 0);
    neoLocal.setPixelColor(i, 0);
  }
  //render all pixels
  digSec01.show();
  digSec10.show();
  digMin01.show();
  digMin10.show();
  digColon.show();
  digHom01.show();
  digHom10.show();
  digVis01.show();
  digVis10.show();
  neoLocal.show();
}
