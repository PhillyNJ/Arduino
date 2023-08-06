#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "bitmap.h"
#include "TeensyTFT.h"

class BitmapHandler {

  public:
    BitmapHandler();
    void init(TeensyTFT* tft, uint8_t _orient);
    void displayBmp(int x1, int y1, const char *bitmap, int pg);
  private:
     File bmFile;
     bool flip;
     uint8_t header_buff[60];
     int position;
     int padding;
     uint8_t orient;
     TeensyTFT *tftRef;
     bitmap_t image;
};

#endif
