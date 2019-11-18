/**
 * 
 * SCOREBOARD - test pattern. Checks for brightness and current draw
 * 
 * BOARD: ARDUINO MEGA 2560
 * 
 * LIBRARIES: Adafruit_NeoPixel
 * 
 */

#include <Adafruit_NeoPixel.h>

#define PIN31          31   //digital pin
#define PIN33          33   //digital pin
#define PIN35          35   //digital pin
#define PIN37          37   //digital pin
#define NUMPIXELS      35   //50 pixel per string
#define BRIGHTNESS     128 



//instantiate a "pixels" object
Adafruit_NeoPixel pixels  = Adafruit_NeoPixel(NUMPIXELS, PIN31, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, PIN33, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(NUMPIXELS, PIN35, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel pixels4 = Adafruit_NeoPixel(NUMPIXELS, PIN37, NEO_RGB + NEO_KHZ800);

//general purpose millisecond timer
unsigned long timer = 0;

/**
 * setup
 */
void setup() {
  
  Serial.begin(115200);
  delay(1000);

  // This initializes the NeoPixel library.
  pixels.begin(); 
  pixels2.begin(); 
  pixels3.begin(); 
  pixels4.begin(); 

  pixels.clear();
  pixels2.clear();
  pixels3.clear();
  pixels4.clear();
  
}//end setup


/**
 * loop
 */
void loop() {

    /*
  //all red   840mA
  for(int i; i< NUMPIXELS; i++){
    pixels2.setPixelColor(i, 255, 0,0);  //n, r, g, b
    pixels2.show();
  }
  delay(2000);
  
  //all green  840mA
  for(int i; i< NUMPIXELS; i++){
    pixels2.setPixelColor(i, 0, 255, 0);  //n, r, g, b
    pixels2.show();
  }
  delay(2000);
  
  //all blue  840mA
  for(int i; i< NUMPIXELS; i++){
    pixels2.setPixelColor(i, 0, 0, 255);  //n, r, g, b
    pixels2.show();
  }
  delay(2000);
  
  //all white  2475mA
  for(int i; i< NUMPIXELS; i++){
    pixels2.setPixelColor(i, 255, 255, 255);  //n, r, g, b
    pixels2.show();
  }
  delay(4000);

*/
  //random colors for n seconds   1250mA
  timer = millis();
  unsigned val = 0;
  while(val < 1000) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, random(0,BRIGHTNESS), random(0,BRIGHTNESS), random(0,BRIGHTNESS));  //n, r, g, b
      pixels.show();
    }
    val = millis() - timer;
  }
  val=0;

  
  //random colors for n seconds   1250mA
  timer = millis();
  while(val < 1000) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels2.setPixelColor(i, random(0,BRIGHTNESS), random(0,BRIGHTNESS), random(0,BRIGHTNESS));  //n, r, g, b
      pixels2.show();
    }
    val = millis() - timer;
  }
  val=0;
  
  //random colors for n seconds   1250mA
  timer = millis();
  while(val < 1000) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels3.setPixelColor(i, random(0,BRIGHTNESS), random(0,BRIGHTNESS), random(0,BRIGHTNESS));  //n, r, g, b
      pixels3.show();
    }
    val = millis() - timer;
  }
  val=0;
  
  //random colors for n seconds   1250mA
  timer = millis();
  while(val < 1000) {
    for(int i = 0; i < NUMPIXELS; i++){
      pixels4.setPixelColor(i, random(0,BRIGHTNESS), random(0,BRIGHTNESS), random(0,BRIGHTNESS));  //n, r, g, b
      pixels4.show();
    }
    val = millis() - timer;
  }
  val=0;


}//end loop


