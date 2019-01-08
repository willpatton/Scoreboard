/**
 * draw score home
 */
void draw_score_home(){

   //clear score home clock pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digHom10.setPixelColor(i, 0);
    digHom01.setPixelColor(i, 0);
   }

   //convert/update the score to 10's:1's digits
   int hom10 = score_home / 10;
   int hom01 = score_home - hom10;
   Serial.print("HOME: ");Serial.print(hom10); Serial.print(hom01);Serial.println();
  
   //render score digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   draw(hom10 + 0x30, 6);
   draw(hom01 + 0x30, 5);
}

/**
 * draw score home
 */
void draw_score_visitors(){

   //clear score visitor clock pixels
   for (int i = 0 ; i < NUMPIXELS; i++) {
    digVis10.setPixelColor(i, 0);
    digVis01.setPixelColor(i, 0);
   }

   //convert/update the score to 10's:1's digits
   int vis10 = score_visitors / 10;
   int vis01 = score_visitors - vis10;
   Serial.print("VISITORS: ");Serial.print(vis10); Serial.print(vis01);Serial.println();
  
   //render score digits in ASCII format (add 0x30 to each numeric value to get ASCII digits)
   draw(vis10 + 0x30, 8);
   draw(vis01 + 0x30, 7);
}

