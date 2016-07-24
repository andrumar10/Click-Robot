/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

with spaces, font 1 is 6x8 pixels, spaces are down and left
basic font display has 168 chars, 6 rows, 14 columns = 84 on screen
font 2 is double everything (1 pixel inflated to 2)
font 3 is triple everything (1 pixel inflated to 3)
width/height is 0 to (maxwidth/maxheight minus one).
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Fonts/FreeSans9pt7b.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

//#define NUMFLAKES 3
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define sections 3

#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH  32

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{   0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 
    0x00, 0x81, 0x80, 0x00, 
    0x00, 0xc1, 0x80, 0x00, 
    0x00, 0xe1, 0xc0, 0x00, 
    0x00, 0xe1, 0xc0, 0x00, 
    0x00, 0xe1, 0xc0, 0x00, 
    0x00, 0xe1, 0xc0, 0x00, 
    0x00, 0xe0, 0x80, 0x00, 
    0x00, 0xe0, 0x80, 0x00, 
    0x00, 0x6f, 0xc0, 0x00, 
    0x00, 0x7f, 0xf0, 0x00, 
    0x00, 0x77, 0xfc, 0x00, 
    0x00, 0x7d, 0xff, 0x00, 
    0x01, 0x7f, 0x7f, 0xc0, 
    0x07, 0x7d, 0xdd, 0xc0, 
    0x07, 0x75, 0xf7, 0x80, 
    0x07, 0x71, 0xff, 0xc0, 
    0x01, 0x7c, 0x7f, 0xc0, 
    0x00, 0x4d, 0x7f, 0x80, 
    0x01, 0xad, 0xff, 0xa0, 
    0x04, 0x2f, 0xdf, 0xe0, 
    0x00, 0x03, 0x4f, 0xe0, 
    0x00, 0x00, 0xef, 0xe0, 
    0x00, 0x00, 0xaa, 0x80, 
    0x00, 0x00, 0xe8, 0x80, 
    0x00, 0x00, 0x40, 0x40, 
    0x00, 0x00, 0x40, 0x00, 
    0x00, 0x00, 0x60, 0x00, 
    0x00, 0x00, 0x20, 0x00, 
    0x00, 0x00, 0x20, 0x00, 
    0x00, 0x00, 0x00, 0x00 };

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50); //scales up to 127

  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

 // testdrawchar();
  /*
  display.display();
  delay(5000);
  display.invertDisplay(true); //THis bullshit isn't subclassed in the LCD Libraries, so it isn't supported. FML.
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo16_glcd_bmp, 32, 32, 1);
  display.display();
  
  testdrawroundrect();

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  display.clearDisplay(); 
  display.display();
*/
  delay(1000);
  backmenu(sections);
 // display.setFont(&FreeSans9pt7b);//something in here is screwing with the cursor setter, would not recommend
  display.setCursor(0,0);
  display.setTextSize(1);
  display.write(0x34);
  display.display();
  delay(1000);
  display.clearDisplay(); //also apparently clears the setcursor!

  //display.drawRoundRect(0, 0, display.width(), display.height(), 4, BLACK);
//  display.drawLine(0, 0, display.width(), display.height(), BLACK);
//  display.drawLine(1,display.height()-2,display.width()-2,1,BLACK);

  display.drawLine(0,12,0,44,BLACK);
  for (uint8_t k=5; k>=1; k--){
      display.clearDisplay();
      display.drawLine(0,display.height()-1,display.width()-k,display.height()-1,BLACK);
      display.drawLine(display.width()-1,0,display.width()-1,display.height()-k,BLACK);
      display.setCursor(k,k);
      display.setTextSize(1);

  //    display.setCursor(display.width()-20,display.height()-20);
      if(k%2 ==0){
     // display.setFont(&FreeSans9pt7b);
      }
      else{
      display.setFont();
      }
      display.write(0x30+k);
      display.display();
      delay(1000);
  }
  display.setCursor(display.width()-20,display.height()-20);

  display.write(66);
   display.write(67);

  display.display();
  delay(10000);

  // draw a bitmap icon and 'animate' movement
//  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);
}


void loop() {
  
}


void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int NUMFLAKES = 3;
  int i = 1;
  uint8_t icons[NUMFLAKES][3];
  randomSeed(analogRead(0));     // proper atmospheric seeding
 
  // initialize    
  Serial.print(display.width(), DEC);

  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(0,display.width()-32); //actual pixel width of image -1 for inclusiveness
    icons[f][YPOS] =  display.height();
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, WHITE);
      // move it
      icons[f][YPOS] -= icons[f][DELTAY];
      icons[f][XPOS] -= icons[f][1];
      Serial.print("x: ");
      Serial.print(icons[f][XPOS], DEC);
      Serial.print(" y: ");
      Serial.print(icons[f][YPOS], DEC);
      Serial.print(" dy: ");
      Serial.println(icons[f][DELTAY], DEC);

      // if its gone, reinit
      
      if (icons[f][YPOS] > 250 || icons[f][XPOS] > 250) { //Gross overflow abuse.
  	    icons[f][XPOS] = random(0,display.width()-32);//actual pixel width of image -1 for inclusiveness
  	    icons[f][YPOS] =  display.height()+LOGO16_GLCD_WIDTH;
  	    icons[f][DELTAY] = random(5) + 1;
        }
      }
   if (i > 10){
     i = 0;
     NUMFLAKES = NUMFLAKES+1;
     }
     i = i+1;
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  delay(3000);
  display.clearDisplay();
  for (uint8_t i=1; i <= 83; i++) { //168 max? is 0 space char or overflow?
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
  delay(3000);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  for (uint8_t i=83; i <= 166; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
  delay(3000);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  for (uint8_t i=166; i < 180; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
  delay(3000);
}

void backmenu(int menuoptions) {
   //int menuoptions = 3;
   int menuline = 0;
   int optionheight = display.height()-12; //border (1) + character height (8) +  pixel space (1) + inline menu border (2)
   display.drawRoundRect(0, 0, display.width(), display.height(), 4, BLACK);
   display.drawLine(42,optionheight,42,display.height()-1,BLACK); //midway on display
   display.drawLine(43,optionheight,43,display.height()-1,BLACK);
   display.setCursor(20-3*3,display.height()-8); // (1/4 the distance - 1) - number of chars * half the width ; for centering
   display.setTextSize(1);
   display.write(0x53); //S
   display.write(0x65); //e
   display.write(0x6C); //l

   display.setCursor(62-3*5,display.height()-8); // (3/4 the distance - 1) - number of chars * half the width ; for centering
   display.write(0x4F); //O
   display.write(0x6B); //k
   display.write(0x00); //' '
   display.write(0x47); //G
   display.write(0x6F); //o
   for(int8_t i=0; i<=menuoptions; i++){
   menuline = (optionheight*i/menuoptions)-1;
    display.drawLine(0,menuline,display.width()-1,menuline,BLACK);
   }
   display.display();
   delay(30000);
} 

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, BLACK);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, BLACK);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/4-2; i+=2) {
    display.drawRoundRect(0, 0, display.width(), display.height(), i, BLACK);
    display.display();
	delay(3000);
	display.clearDisplay();
  }
}

void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, BLACK);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, BLACK);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, BLACK);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, BLACK);
    display.display();
  }
  for (int8_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, BLACK);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, BLACK);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, BLACK);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, BLACK);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, BLACK); 
    display.display();
  }
  delay(250);
}
