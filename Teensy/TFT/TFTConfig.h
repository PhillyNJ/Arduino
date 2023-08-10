/*
Created by Philip Vallone
Ported from the UTFT Library http://www.rinkydinkelectronics.com/library.php?id=51
and modified for the Teensy 4.1

** TFT Display for the following TFTs **

* SSD1989 320X240 (3.5 inch 320X240)- 16 bit
* SSD1963 400x272 (4.3 inch 320X240)- 16 bit
* SSD1963_800_5IN (5.0 inch 800X480)- 16 bit
* SSD1963_800  (7.0 inch 800X480)- 16 bit
* ILI9325D_8 (2.4 inch 320X240)- 8 bit
* ILI9325D_16 (2.8 inch 320X240)- 16 bit
* ILI9341 (2.4 inch 320X240)- 8 bit

Reads bmp and raw images.

* Paging only works on SSD1963_480 

*/

/* Pin out

Teensy		8 bit TFT	16 bit TFT
-------------------------------------
38		RS		RS
39		WR		WR
40		CS		CS
41		RS_T		RS_T (RESET)


0		8		DB00
1		9		DB01
2		10		DB02
3		11		DB03
4		12		DB04
5		13		DB05
6		14		DB06
7		15		DB07

8		----		DB08
9		----		DB09
10		----		DB10
11		----		DB11
12		----		DB12
13		----		DB13
14		----		DB14
15		----		DB15

SPI For Touch on EXT2
------------------------------------
Teensy		TFT		Function
------------------------------------
A6 (20) 	T_CS		SS
A7 (21)		T_DIN		MOSI
A8 (22)		T_DO		MISO
A9 (23)		T_CLK		SCK

*/



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
