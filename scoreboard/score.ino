/**
 * score
 * 
 * 
 * @author:   Will Patton 
 * @url:      http://github.com/willpatton
 * @license:  MIT License
 * 
 * Scoreboard
 * Renders the score
 *  
 */

/*
 * SCOREBOARD INIT - puts scoreboard in to a known state 
 */
void scoreboard_init(){

    Serial.println("SCOREBOARD init: ");
    
    //rendor scores
    draw_score_home();      
    draw_score_visitors();  

    //clear flags
    //flag_home = 0;
    //flag_visitors = 0;
    flag_scoreboard = 0;
    
}


/**
 * draw score home
 */
void draw_score_home(){

   //clear pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digHom10.setPixelColor(i, 0);
    digHom01.setPixelColor(i, 0);
   }

   //convert/update the score to 10's:1's digits
   int hom10 = score_home / 10;
   int hom01 = score_home - (hom10 * 10);
   Serial.print("HOME: ");Serial.print(hom10);Serial.print(" ");Serial.print(hom01);Serial.println();
   //render score digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   draw(hom10 + 0x30, 6);
   draw(hom01 + 0x30, 5);

   //flag_home = 0;       //clear flag
}


/**
 * draw score visitors
 */
void draw_score_visitors(){

   //clear pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digVis10.setPixelColor(i, 0);
    digVis01.setPixelColor(i, 0);
   }

   //convert/update the score to 10's:1's digits
   int vis10 = score_visitors / 10;
   int vis01 = score_visitors - (vis10 * 10);
   Serial.print("VISITORS: ");Serial.print(vis10); Serial.print(" ");Serial.print(vis01);Serial.println();
  
   //render score digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   draw(vis10 + 0x30, 8);
   draw(vis01 + 0x30, 7);

   //flag_visitors = 0;       //clear flag
}
