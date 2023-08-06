#ifndef TEENSYTFT_H
#define TEENSYTFT_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "TFTConfig.h"


class TeensyTFT {

  public:
    TeensyTFT();
    void initTFT(struct tft_chip tftConfig);
    void setPinIO();
    void setRST();
    void clearRST();
    void clearRs();
    void setRs();
    void clearCs();
    void setCs();
    void write16ToTFT(uint8_t vh, uint8_t vl);
    void pulseBitLow();
    void writeData(uint8_t vh, uint8_t vl);
    void writeBus(uint8_t hv, uint8_t lv);
    void writeByteData(uint8_t data);
    void writeComData(uint8_t com, uint16_t data);
    void setXY(int x1, int y1, int x2, int y2);
    void setPixel(uint8_t r, uint8_t g, uint8_t b);
    void clrXY();
    void clrScr();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void writeCom(uint8_t com);
    void writeToTFT(uint8_t vl);
    void setBackColor(uint8_t r, uint8_t g, uint8_t b);
    void setFont(const unsigned char* f, const unsigned int (*d)[3], uint8_t c);
    void drawHLine(int x, int y, int l);
    void drawHLinePage(int x, int y, int l, int page);

    void drawVLine(int x, int y, int l);
    void drawVLinePage(int x, int y, int len, int page);
    void drawLine(int x1, int y1, int x2, int y2);

    void setCharMap(const unsigned char* cm);
    uint8_t getIndex(char c);
    void printChar(char ch, int x, int y);
    void print(const char *st, int x, int y);
    void setTFTProperties();
    void scroll(int y);
    void setScrollArea(uint16_t top, uint16_t scr, uint16_t bottom);
    void setPage(int pageNumber);
    void load_raw_image_mem(int x1, int y1, int imgX, int imgY, const uint16_t *img, int size, int page);
    void load_raw_image_mem(int x1, int y1, int imgX, int imgY, const uint16_t *img, int size, int deg, int rox, int roy, int page) ;
    void printText(uint16_t x, uint16_t y, const char* fmt, ... );
    void printTextPage(int page, uint16_t x, uint16_t y, const char* fmt, ... );

    void drawPixel(int x, int y);
    void fillPage(uint8_t r, uint8_t g, uint8_t b, int page);
    void fillScr(uint8_t r, uint8_t g, uint8_t b);
    void fillRoundRect(int x1, int y1, int x2, int y2);
    void fillRoundRect(int x1, int y1, int x2, int y2, int page);
    
    void drawRoundRect(int x1, int y1, int x2, int y2);
     void drawRoundRect(int x1, int y1, int x2, int y2, int page);

    void fillRec(int x1, int y1, int x2, int y2);
    void fillRec(int x1, int y1, int x2, int y2, int page);
    void drawCircle(int x, int y, int radius);
    void drawCircle(int x, int y, int radius, int page);
    void fillCircle(int x, int y, int radius);
    void fillCircle(int x, int y, int radius, int page);
  private:
    struct tft_chip config;
    File file;
    uint8_t fcolorr, fcolorg, fcolorb;
    uint8_t bcolorr, bcolorg, bcolorb;
    struct fontSet fi;
    struct charInfo ci;
    struct currentFont cfont;
    const unsigned char* cMap;
    int current_x, current_y;
};

#endif
