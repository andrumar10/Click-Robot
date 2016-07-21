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
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

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
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(100);
  display.clearDisplay();   // clears the screen and buffer

  testdrawchar();
  display.display();
  delay(5000);
 while(1){
  display.invertDisplay(true); //THis bullshit isn't subclassed in the LCD Libraries, so it isn't supported. FML.
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  }
  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo16_glcd_bmp, 32, 32, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  display.clearDisplay(); 
  display.display();

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);
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

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
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
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, BLACK);
    display.display();
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
