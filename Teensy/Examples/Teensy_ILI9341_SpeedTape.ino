/* Works freaking awesome! 
******************************
Uses the ILI9341_t3n -> https://github.com/KurtE/ILI9341_t3n
Pinout
Teensy 4.1        ILI9341 TFT
SCK               13
MISO              12
MOSI              11
CS                10
DS/DC             9
3.3v              3.3v
GND               GND
3.3v              Reset

14 (A0)           Connect POT to 
*/
#include "SPI.h"
#include "ILI9341_t3n.h"
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10
#define REDRAW_DELAY 16  // minimum delay in milliseconds between display updates
#define swap(type, i, j) \
  { \
    type t = i; \
    i = j; \
    j = t; \
  }
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);
unsigned long redrawTime = 0;
DMAMEM uint16_t fb1[320 * 240];

int pot = 0;
int speedReading = 0;
int altitudeReading = 0;
int orientation = 3;
bool isPortrait = orientation % 2 == 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  tft.begin();
  tft.setRotation(orientation);
  tft.setTextSize(1);
  tft.setFrameBuffer(fb1);
  tft.useFrameBuffer(true);
  tft.fillScreen(ILI9341_BLACK);
  updateSpeedTapeV3(0, 1);
  tft.updateScreen();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() > redrawTime) {
    pot = analogRead(A0);
    speedReading = map(pot, 0, 1023, 0, 650);
    altitudeReading = map(pot, 0, 1023, 0, 10000);
    redrawTime = millis() + REDRAW_DELAY;    
    updateSpeedTapeV3(speedReading, 1);
    updateAltitudeTape(altitudeReading, 1);
    tft.updateScreen();
  }
}

/*
  Examples supports orientation and scaling
  Also, addes odometer type display
*/
void updateSpeedTapeV3(int speed, uint8_t scale) {
  
  int ones = 0;
  int tens = 0;
  int hundreds = 0;
  
  if (scale < 1) {
    scale = 1;
  }
  int width = 240;
  int height = 320; 
  int offset = 160 * scale;
  if (!isPortrait) {
    offset = 120 * scale;
    swap(int, width, height);
  }
  int min = speed - offset;
  int max = speed + offset;  // max speed
  int y = 0;

  tft.fillRect(0, 0, 70, height, ILI9341_DARKGREY);
  tft.drawFastVLine(70, 0, height, ILI9341_WHITE);

  for (int t = min; t < max * scale; t++) {

    if (t / scale >= 0) {
      y = map(t, max, min, 0, height);

      if (t % 50 == 0) {
        tft.drawLine(2, y, 20, y, ILI9341_WHITE);
        if (y - 7 > 0) {
          printT(23, y - 7, 2, ILI9341_WHITE, "%d", t);
        }
      }
      if (t % 10 == 0) {
        tft.drawLine(2, y, 10, y, ILI9341_WHITE);
      }

      if (t > 140) {  // over speed
        tft.drawLine(65, y, 70, y, ILI9341_RED);
      } else if (t > 110) {
        tft.drawLine(65, y, 70, y, ILI9341_YELLOW);
      } else {
        tft.drawLine(65, y, 70, y, ILI9341_GREEN);
      }
    }
  }
  tft.fillRect(15, height / 2 - 10, 46, 20, ILI9341_BLACK);
  tft.fillTriangle(70, height / 2, 60, height / 2 - 10, 60, height / 2 + 9, ILI9341_BLACK);
  
  tft.fillRect(45, height/2 - 30, 20, 55, ILI9341_BLACK);
 
  if(speed > 0){
    ones = speed % 10;
    if(ones == 10){
      ones = 0;
    }
  } else {
    ones = 0;
  }    
  speed /= 10;
  tens = speed % 10;
  speed /= 10;
  hundreds = speed % 10;   
  int prev = ones - 1;
  if(prev < 0){
    prev = 0;
  }
  int next = ones + 1;
  if(next > 9){
    next = 0;
  }
  printT(47, height / 2 - 25 - ones, 2, ILI9341_WHITE, "%d", next);
  printT(47, height / 2 - 7 - ones, 2, ILI9341_WHITE, "%d", ones);
  printT(47, height / 2 + 13 - ones, 2, ILI9341_WHITE, "%d", prev);
  // mask other over flow  
  tft.fillRect(45, height / 2 - 34, 20, 9, ILI9341_DARKGREY);
  tft.fillRect(45, height / 2 + 20, 20, 9, ILI9341_DARKGREY);

  // print the finals left most numbers
  printT(22, height / 2 - 7, 2, ILI9341_WHITE, "%d%d", hundreds, tens);
}

void updateAltitudeTape(int altitude, uint8_t scale) {

  if (scale < 1) {
    scale = 1;
  }
  int width = 240;
  int height = 320;

  int offset = 160 * scale;
  if (orientation == 1 || orientation == 3) {
    offset = 120 * scale;
    swap(int, width, height);
  }
  int min = altitude - offset;
  int max = altitude + offset;  // max speed
  int y = 0;
  int xstart = width - 100;
  tft.fillRect(xstart, 0, 100, height, ILI9341_DARKGREY);
  tft.drawFastVLine(xstart, 0, height, ILI9341_WHITE);

  for (int t = min; t < max * scale; t++) {
    if (t / scale >= 0) {
      y = map(t, max, min, 0, height);
      if (t % 100 == 0) {
        tft.drawLine(xstart, y, xstart + 20, y, ILI9341_WHITE);
        if (y - 7 > 0) {
          printT(xstart + 23, y - 7, 2, ILI9341_WHITE, "%d", t);
        }
      }
      if (t % 100 != 0 && t % 50 == 0) {
        printT(width - 25, y - 3, 1, ILI9341_WHITE, "50");
      }
      if (t % 100 != 0 && t % 50 != 0 && t % 25 == 0 && t % 75 == 0) {
        printT(width - 25, y - 3, 1, ILI9341_WHITE, "75");
      }
      if (t % 100 != 0 && t % 50 != 0 && t % 25 == 0 && t % 75 != 0 && t % 25 == 0) {
        printT(width - 25, y - 3, 1, ILI9341_WHITE, "25");
      }
      if (t % 10 == 0) {
        tft.drawLine(xstart + 2, y, xstart + 10, y, ILI9341_WHITE);

        tft.drawLine(width - 10, y, width - 5, y, ILI9341_WHITE);
      }
      if (t % 5 == 0) {

        tft.drawLine(width - 8, y, width - 5, y, ILI9341_WHITE);
      }
    }
  }
  tft.fillRect(xstart + 13, height / 2 - 10, 76, 20, ILI9341_BLACK);
  tft.fillTriangle(xstart, height / 2, xstart + 13, height / 2 - 10, xstart + 13, height / 2 + 9, ILI9341_BLACK);
  tft.fillTriangle(width - 5, height / 2, width - 10, height / 2 - 5, width - 10, height / 2 + 5, ILI9341_WHITE);
  printT(xstart + 22, height / 2 - 7, 2, ILI9341_WHITE, "%d", altitude);
}

/*
  A very basic example, does not support scale or switching orientation
*/
void updateSpeedTape(int speed) {
  int min = speed - 110;
  int max = speed + 110;  // max speed
  int y = 0;

  tft.fillRect(0, 0, 70, tft.height(), ILI9341_DARKGREY);
  tft.drawFastVLine(70, 0, tft.height(), ILI9341_WHITE);

  for (int t = min; t < max; t++) {

    if (t >= 0) {
      y = map(t, max, min, 0, 320);
      if (t % 50 == 0) {
        tft.drawLine(2, y, 20, y, ILI9341_WHITE);
        if (y - 7 > 0) {
          printT(23, y - 7, 2, ILI9341_WHITE, "%d", t);
        }
      }
      if (t % 10 == 0) {
        tft.drawLine(2, y, 10, y, ILI9341_WHITE);
      }
      if (t > 140) {  // over speed
        tft.drawLine(65, y, 70, y, ILI9341_RED);
      } else if (t > 110) {
        tft.drawLine(65, y, 70, y, ILI9341_YELLOW);
      } else {
        tft.drawLine(65, y, 70, y, ILI9341_GREEN);
      }
    }
  }
  tft.fillRect(15, tft.height() / 2 - 10, 46, 20, ILI9341_BLACK);
  tft.fillTriangle(70, tft.height() / 2, 60, tft.height() / 2 - 10, 60, tft.height() / 2 + 10, ILI9341_BLACK);
  printT(22, tft.height() / 2 - 7, 2, ILI9341_WHITE, "%03d", speed);
}
/*
  Examples supports orientation
*/
void updateSpeedTapeV2(int speed) {

  int width = 240;
  int height = 320;
  int offset = 160;
  if (orientation == 1 || orientation == 3) {
    offset = 120;
    swap(int, width, height);
  }
  int min = speed - offset;
  int max = speed + offset;  // max speed
  int y = 0;

  tft.fillRect(0, 0, 70, height, ILI9341_DARKGREY);
  tft.drawFastVLine(70, 0, height, ILI9341_WHITE);

  for (int t = min; t < max; t++) {
    if (t >= 0) {
      y = map(t, max, min, 0, height);
      if (t % 50 == 0) {
        tft.drawLine(2, y, 20, y, ILI9341_WHITE);
        if (y - 7 > 0) {
          printT(23, y - 7, 2, ILI9341_WHITE, "%d", t);
        }
      }
      if (t % 10 == 0) {
        tft.drawLine(2, y, 10, y, ILI9341_WHITE);
      }
      if (t > 140) {  // over speed
        tft.drawLine(65, y, 70, y, ILI9341_RED);
      } else if (t > 110) {
        tft.drawLine(65, y, 70, y, ILI9341_YELLOW);
      } else {
        tft.drawLine(65, y, 70, y, ILI9341_GREEN);
      }
    }
  }
  tft.fillRect(15, height / 2 - 10, 46, 20, ILI9341_BLACK);
  tft.fillTriangle(70, height / 2, 60, height / 2 - 10, 60, height / 2 + 10, ILI9341_BLACK);
  printT(22, height / 2 - 7, 2, ILI9341_WHITE, "%03d", speed);
}

void printT(int x, int y, int size, int color, const char *format, ...) {
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.setTextColor(color);
  va_list arglist;
  char dest[80];  // ttoo much?
  va_start(arglist, format);
  vsprintf(dest, format, arglist);
  va_end(arglist);
  tft.println(dest);
}