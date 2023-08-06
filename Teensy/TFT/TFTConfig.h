#ifndef TFTCONFIG_H
#define TFTCONFIG_H

#include <stdint.h>
#include <stdio.h>


#define GPIO6_MASK 0x303C000C //0011 0000 0011 1100 0000 0000 0000 1100 - pins 0,1,14,15, pins 38-39, 40-41  right to left
#define GPIO7_MASK 0x330C0F   //0000 0000 0000 0011 0000 1100 0000 1111 - pins 6-13,
#define GPIO8_MASK 0x00000000 // 0000 0000 0000 0000 0000 0000 0000 0000 - 
#define GPIO9_MASK 0x00000170 //0000 0000 0000 0000 0000 0001 0111 0000 - pin 2-5

#define             MAXSTRINGSIZE 1024
#define RS          CORE_PIN38_BIT // normally HIGH
#define WR          CORE_PIN39_BIT // normally HIGH
#define CS          CORE_PIN40_BIT // normally LOW
#define RS_T        CORE_PIN41_BIT // normally HIGH

#define swap(type, i, j) {type t = i; i = j; j = t;}


#define       PORTRAIT 0
#define       LANDSCAPE 1


enum tft_type {
  ILI9325D_8 = 0,
  ILI9325D_16 = 1,
  SSD1963_480 = 2,
  SSD1289 = 3,
  SSD1963_800 = 4,
  SSD1963_800_5IN = 5,
  ILI9341 = 6
};

struct tft_chip {
  enum tft_type tft_model;
  uint8_t pages;
  uint8_t orient;
  int width;
  int height;
  int page_1; // start address of page
  int page_2;
  int page_3;
};

struct currentFont
{
  uint8_t* font;
  uint8_t x_size;
  uint8_t y_size;
  uint8_t offset;
  uint8_t numchars;
};

struct charInfo {

  int width;
  int offset;

};

struct fontSet {

  const unsigned char* font; // your font array
  const unsigned int (*descriptors)[3]; // your font info
  uint8_t num_of_chars; // number of characters in your font set
  uint8_t rows;

};


#endif
