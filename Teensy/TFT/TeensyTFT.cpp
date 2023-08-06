#include "TeensyTFT.h"
#include <stdarg.h>


TeensyTFT::TeensyTFT() {
  // constructor
}

void TeensyTFT::initTFT(struct tft_chip tftConfig) {
  config = tftConfig;
  setPinIO();

  delayMicroseconds(5);
  setRST();
  delayMicroseconds(1);
  clearRST();
  delayMicroseconds(1);
  setRST();
  delayMicroseconds(1);
  clearCs();

  delayMicroseconds(1);
  setTFTProperties();

}


void TeensyTFT::setPinIO() {

  GPIO6_GDIR = GPIO6_MASK;
  GPIO7_GDIR = GPIO7_MASK;
  GPIO8_GDIR = GPIO8_MASK;
  GPIO9_GDIR = GPIO9_MASK;
}



void TeensyTFT::clearRST() {
  GPIO6_DR_CLEAR = (1 << (RS_T));  // clear bit
}
void TeensyTFT::setRST() {
  GPIO6_DR_SET = (1 << (RS_T));  // set bit
}

void TeensyTFT::clearRs() {
  GPIO6_DR_CLEAR = (1 << (RS));  // clear bit
}
void TeensyTFT::setRs() {
  GPIO6_DR_SET = (1 << (RS));  // set bit
}

void TeensyTFT::clearCs() {
  GPIO6_DR_CLEAR = (1 << (CS));  // clear bit
}
void TeensyTFT::setCs() {
  GPIO6_DR_SET = (1 << (CS));  // set
}

void TeensyTFT::write16ToTFT(uint8_t vh, uint8_t vl) {
  // clear the registors

  GPIO6_DR_CLEAR = (1 << (CORE_PIN0_BIT)) | (1 << (CORE_PIN1_BIT)) | (1 << (CORE_PIN14_BIT)) | (1 << (CORE_PIN15_BIT));
  GPIO7_DR_CLEAR = (1 << (CORE_PIN6_BIT)) | (1 << (CORE_PIN7_BIT)) | (1 << (CORE_PIN8_BIT)) |  (1 << (CORE_PIN9_BIT)) | (1 << (CORE_PIN10_BIT)) | (1 << (CORE_PIN11_BIT)) | (1 << (CORE_PIN12_BIT)) |  (1 << (CORE_PIN13_BIT));
  GPIO9_DR_CLEAR = (1 << (CORE_PIN2_BIT)) | (1 << (CORE_PIN3_BIT)) | (1 << (CORE_PIN4_BIT)) | (1 << (CORE_PIN5_BIT));  // 2-5
  delayMicroseconds(1);
  // Pins 0, 1, 14,15
  GPIO6_DR_SET = (((vl >> 0) & 1) << (CORE_PIN0_BIT)) | (((vl >> 1) & 1) << (CORE_PIN1_BIT)) | (((vh >> 6) & 1) << (CORE_PIN14_BIT)) | (((vh >> 7) & 1) << (CORE_PIN15_BIT));

  // 6-13 on GPIO7_DR_SET
  GPIO7_DR_SET = (((vl >> 6) & 1) << (CORE_PIN6_BIT)) | (((vl >> 7) & 1) << (CORE_PIN7_BIT)) | (((vh >> 0) & 1) << (CORE_PIN8_BIT)) | (((vh >> 1) & 1) << (CORE_PIN9_BIT)) | (((vh >> 2) & 1) << (CORE_PIN10_BIT)) | (((vh >> 3) & 1) << (CORE_PIN11_BIT)) | (((vh >> 4) & 1) << (CORE_PIN12_BIT)) | (((vh >> 5) & 1) << (CORE_PIN13_BIT));

  // 2-5
  GPIO9_DR_SET = (((vl >> 2) & 1) << (CORE_PIN2_BIT)) | (((vl >> 3) & 1) << (CORE_PIN3_BIT)) | (((vl >> 4) & 1) << (CORE_PIN4_BIT)) | (((vl >> 5) & 1) << (CORE_PIN5_BIT));

}
void TeensyTFT::writeComData(uint8_t com, uint16_t data) {

  writeCom(com);
  writeData(data >> 8, data);

}

void TeensyTFT::writeToTFT(uint8_t vl) {
  // not supported yet - this is for 8 bit bus
  //REG_PORT_OUTCLR1 = PORTBMASK_16;
  //REG_PORT_OUTSET1 = (((vl) & 0xFF));
}

void TeensyTFT::pulseBitLow() {

  GPIO6_DR_CLEAR = (1 << (WR));  // clear bit
  GPIO6_DR_SET = (1 << (WR));    // set bit

}

void TeensyTFT::writeBus(uint8_t hv, uint8_t lv) {

  write16ToTFT(hv, lv);
  pulseBitLow();
}

void TeensyTFT::writeData(uint8_t vh, uint8_t vl) {

  setRs();
  write16ToTFT(vh, vl);
  pulseBitLow();
}

void TeensyTFT::writeByteData(uint8_t data) {

  setRs();
  write16ToTFT(0x00, data);

  pulseBitLow();
}

void TeensyTFT::setPixel(uint8_t r, uint8_t g, uint8_t b) {
  writeData(((r & 248) | g >> 5), ((g & 28) << 3 | b >> 3));
}

void TeensyTFT::setXY(int x1, int y1, int x2, int y2) {
  if (config.orient == LANDSCAPE) {
    swap(int, x1, y1);
    swap(int, x2, y2);
    y1 = config.height - y1;
    y2 = config.height - y2;
    swap(int, y1, y2)
  }


  swap(int, x1, y1);
  swap(int, x2, y2);
  writeCom(0x2a);
  writeByteData(x1 >> 8);
  writeByteData(x1);
  writeByteData(x2 >> 8);
  writeByteData(x2);
  writeCom(0x2b);
  writeByteData(y1 >> 8);
  writeByteData(y1);
  writeByteData(y2 >> 8);
  writeByteData(y2);
  writeCom(0x2c);
}

void TeensyTFT::clrXY() {
  if (config.orient == PORTRAIT) {

    setXY(0, 0, config.width, config.height);
  } else {
    setXY(0, 0, config.height, config.width);
  }
}


void TeensyTFT::clrScr() {

  int i;

  clearCs();

  clrXY();

  setRs();
  int pixels = (config.width + 1) * (config.height + 1);

  for (i = 0; i < pixels; i++) {
    writeBus(0, 0);
  }
  setCs();
}

void TeensyTFT::setColor(uint8_t r, uint8_t g, uint8_t b) {
  fcolorr = r;
  fcolorg = g;
  fcolorb = b;
}

void TeensyTFT::writeCom(uint8_t com) {

  clearRs();
  writeBus(0x00, com);

}

void TeensyTFT::setBackColor(uint8_t r, uint8_t g, uint8_t b) {
  bcolorr = r;
  bcolorg = g;
  bcolorb = b;
}

void TeensyTFT::setFont(const unsigned char* f, const unsigned int (*d)[3], uint8_t c) {

  fi.font = f;
  fi.descriptors = d;
  fi.num_of_chars = c;
  fi.rows = (int)fi.descriptors[0][1];  // get height for characters
}

/* Draw functions */

void TeensyTFT::drawHLine(int x, int y, int l) {
  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();  //clearBit(CS);
  setXY(x, y, x + l, y);
  for (int i = 0; i < l + 1; i++) {
    writeData(ch, cl);
  }
  setCs();  //setBit(CS);
  clrXY();
}

void TeensyTFT::drawHLinePage(int x, int y, int l, int page) {

  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();  //clearBit(CS);
  setXY(x, y + page, x + l, y + page);
  for (int i = 0; i < l + 1; i++) {
    writeData(ch, cl);
  }
  setCs();  //setBit(CS);
  clrXY();
}

void TeensyTFT::drawLine(int x1, int y1, int x2, int y2) {
  double delta, tx, ty;

  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  if (((x2 - x1) < 0)) {
    swap(int, x1, x2);
    swap(int, y1, y2);
  }
  if (((y2 - y1) < 0)) {
    swap(int, x1, x2);
    swap(int, y1, y2);
  }

  if (y1 == y2) {
    if (x1 > x2) {
      swap(int, x1, x2);
    }
    drawHLine(x1, y1, x2 - x1);
  } else if (x1 == x2) {
    if (y1 > y2) {
      swap(int, y1, y2);
    }
    drawVLine(x1, y1, y2 - y1);
  } else if (abs(x2 - x1) > abs(y2 - y1)) {
    clearCs();  //clearBit(CS);
    delta = ((double)(y2 - y1) / (double)(x2 - x1));
    ty = (double)y1;
    if (x1 > x2) {
      for (int i = x1; i >= x2; i--) {
        setXY(i, (int)(ty + 0.5), i, (int)(ty + 0.5));
        writeData(ch, cl);
        ty = ty - delta;
      }
    } else {
      for (int i = x1; i <= x2; i++) {
        setXY(i, (int)(ty + 0.5), i, (int)(ty + 0.5));
        writeData(ch, cl);
        ty = ty + delta;
      }
    }
    setCs();  //setBit(CS);
  } else {
    clearCs();  //clearBit(CS);
    delta = ((float)(x2 - x1) / (float)(y2 - y1));
    tx = (float)x1;
    if (y1 > y2) {
      for (int i = y2 + 1; i > y1; i--) {
        setXY((int)(tx + 0.5), i, (int)(tx + 0.5), i);
        writeData(ch, cl);
        tx = tx + delta;
      }
    } else {
      for (int i = y1; i < y2 + 1; i++) {
        setXY((int)(tx + 0.5), i, (int)(tx + 0.5), i);
        writeData(ch, cl);
        tx = tx + delta;
      }
    }
    setCs();  // setBit(CS);
  }
  clrXY();
}

void TeensyTFT::drawVLine(int x, int y, int l) {
  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();  //clearBit(CS);
  setXY(x, y, x, y + l);
  for (int i = 0; i < l; i++) {
    writeData(ch, cl);
  }
  setCs();  //setBit(CS);
  clrXY();
}

void TeensyTFT::drawVLinePage(int x, int y, int len, int page) {
  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();  //clearBit(CS);
  setXY(x, y + page, x, y + len + page);
  for (int i = 0; i < len; i++) {
    writeData(ch, cl);
  }
  setCs();  //setBit(CS);
  clrXY();
}

void TeensyTFT::setCharMap(const unsigned char* cm) {
  cMap = cm;
}

uint8_t TeensyTFT::getIndex(char c) {

  for (int i = 0; i < 96; i++) {

    if (c == (int)cMap[i]) {

      return i;
    }
  }
  return 0;
}

void TeensyTFT::printChar(char ch, int x, int y) {

  clearCs();
  uint8_t bitCt = 8;  // default
  uint8_t indx;

  indx = getIndex(ch);  // get the index from the char map

  ci.width = (int)(fi.descriptors[indx][0]);
  ci.offset = (int)(fi.descriptors[indx][2]);

  if (ci.width < 9) {
    bitCt = 8;
  } else if (ci.width < 17) {
    bitCt = 16;
  } else if (ci.width < 25) {
    bitCt = 24;
  } else if (ci.width < 33) {
    bitCt = 32;
  } else if (ci.width < 41) {
    bitCt = 40;
  } else if (ci.width < 49) {
    bitCt = 48;
  } else if (ci.width < 57) {
    bitCt = 56;

  } else if (ci.width < 65) {
    bitCt = 64;
  }

  int nextIndx = 0;

  if (indx + 1 >= fi.num_of_chars) {  // array size 95

    nextIndx = ci.offset + (int)fi.descriptors[1][2] - (int)fi.descriptors[0][2];

  } else {

    nextIndx = (int)(fi.descriptors[indx + 1][2]);
  }

  for (int i = ci.offset; i < nextIndx; i = i + bitCt / 8) {

    if (bitCt == 16) {

      if (config.orient == PORTRAIT) {

        setXY(current_x, y, current_x + ci.width, y);

        uint8_t byte1 = (uint8_t)fi.font[i];
        uint8_t byte2 = (uint8_t)fi.font[i + 1];

        uint32_t chr = (byte2 << 8) + byte1;

        for (int ii = 0; ii < ci.width; ii++) {

          if ((chr >> ii) % 2 != 0) {

            setPixel(fcolorr, fcolorg, fcolorb);

          } else {
            setPixel(bcolorr, bcolorg, bcolorb);
          }
        }


      } else {  // LANDSCAPE

        setXY(current_x, y, current_x + ci.width, y);

        uint8_t byte1 = (uint8_t)fi.font[i];
        uint8_t byte2 = (uint8_t)fi.font[i + 1];

        uint32_t chr = (byte2 << 8) + byte1;

        for (int ii = ci.width; ii > -1; ii--) {

          if ((chr >> ii) % 2 != 0) {
            //  printf("X");
            setPixel(fcolorr, fcolorg, fcolorb);

          } else {

            //printf("0");
            setPixel(bcolorr, bcolorg, bcolorb);
          }
        }
      }



    } else {

      if (config.orient == PORTRAIT) {

        setXY(current_x, y, current_x + ci.width, y);

        for (int r = 0; r < bitCt / 8; r++) {

          uint32_t chr = (uint8_t)fi.font[i + r];

          for (int ii = 7; ii > -1; ii--) {

            if ((chr & (1 << (7 - ii))) != 0) {

              setPixel(fcolorr, fcolorg, fcolorb);

            } else {

              setPixel(bcolorr, bcolorg, bcolorb);
            }
          }
        }

      } else {

        setXY(current_x, y, current_x + ci.width, y);

        for (int r = bitCt / 8 - 1; r > -1; r--) {

          uint32_t chr = (uint8_t)fi.font[i + r];

          for (int ii = 0; ii < 8; ii++) {

            if ((chr & (1 << (7 - ii))) != 0) {

              setPixel(fcolorr, fcolorg, fcolorb);

            } else {

              setPixel(bcolorr, bcolorg, bcolorb);
            }
          }
        }
      }
    }
    y++;
  }

  setCs();  //digitalWrite(CS, HIGH);
  clrXY();
}

void  TeensyTFT::print(const char* st, int x, int y) {
  int stl, i;
  stl = strlen(st);
  current_x = x;
  current_y = y;

  for (i = 0; i < stl; i++) {

    printChar(*st++, x, y);
    current_x = current_x + ci.width;
    current_y = y;
  }
}

void TeensyTFT::setTFTProperties() {

  writeCom(0xE2);       //PLL multiplier, set PLL clock to 120M
  delayMicroseconds(1);
  writeByteData(0x23);  //N=0x36 for 6.5M, 0x23 for 10M crystal
  delayMicroseconds(1);
  writeByteData(0x02);
  delayMicroseconds(1);
  writeByteData(0x54);
  delayMicroseconds(1);
  writeCom(0xE0);  // PLL enable
  delayMicroseconds(10);
  writeByteData(0x01);
  delayMicroseconds(100);
  writeCom(0xE0);
  delayMicroseconds(1);
  writeByteData(0x03);
  delayMicroseconds(1);
  writeCom(0x01);  // software reset
  delayMicroseconds(1);
  writeCom(0xE6);  //PLL setting for PCLK, depends on resolution
  delayMicroseconds(1);
  writeByteData(0x01);
  delayMicroseconds(1);
  writeByteData(0x1F);
  delayMicroseconds(1);
  writeByteData(0xFF);
  delayMicroseconds(1);
  writeCom(0xB0);  //LCD SPECIFICATION
  delayMicroseconds(1);
  writeByteData(0x20);
  delayMicroseconds(1);
  writeByteData(0x00);
  delayMicroseconds(1);
  writeByteData(0x01);  //Set HDP 479
  delayMicroseconds(1);
  writeByteData(0xDF);
  delayMicroseconds(1);
  writeByteData(0x01);  //Set VDP 271
  delayMicroseconds(1);
  writeByteData(0x0F);
  delayMicroseconds(1);
  writeByteData(0x00);
  delayMicroseconds(1);
  writeCom(0xB4);       //HSYNC
  delayMicroseconds(1);
  writeByteData(0x02);  //Set HT  531
  delayMicroseconds(1);
  writeByteData(0x13);
  delayMicroseconds(1);
  writeByteData(0x00);  //Set HPS 8
  delayMicroseconds(1);
  writeByteData(0x08);
  delayMicroseconds(1);
  writeByteData(0x2B);  //Set HPW 43
  delayMicroseconds(1);
  writeByteData(0x00);  //Set LPS 2
  delayMicroseconds(1);
  writeByteData(0x02);
  delayMicroseconds(1);
  writeByteData(0x00);
  delayMicroseconds(1);
  writeCom(0xB6);       //VSYNC
  delayMicroseconds(1);
  writeByteData(0x01);  //Set VT  288
  delayMicroseconds(1);
  writeByteData(0x20);
  delayMicroseconds(1);
  writeByteData(0x00);  //Set VPS 4
  delayMicroseconds(1);
  writeByteData(0x04);
  delayMicroseconds(1);
  writeByteData(0x0c);  //Set VPW 12
  delayMicroseconds(1);
  writeByteData(0x00);  //Set FPS 2
  delayMicroseconds(1);
  writeByteData(0x02);
  delayMicroseconds(1);
  writeCom(0xBA);
  delayMicroseconds(1);
  writeByteData(0x0F);  //GPIO[3:0] out 1
  delayMicroseconds(1);
  writeCom(0xB8);
  delayMicroseconds(1);
  writeByteData(0x07);  //GPIO3=input, GPIO[2:0]=output
  delayMicroseconds(1);
  writeByteData(0x01);  //GPIO0 normal
  delayMicroseconds(1);
  writeCom(0x36);       //rotation
  delayMicroseconds(1);
  writeByteData(0x22);
  delayMicroseconds(1);
  writeCom(0xF0);  //pixel data interface
  delayMicroseconds(1);
  writeByteData(0x03);
  delayMicroseconds(1);
  writeCom(0x3A);
  delayMicroseconds(1);
  writeByteData(0x05);  // Set Pixel Format
  delayMicroseconds(1);
  setXY(0, 0, 479, 271);
  delayMicroseconds(1);
  writeCom(0x29);  //display on
  delayMicroseconds(1);
  writeCom(0xBE);  //set PWM for B/L
  delayMicroseconds(1);
  writeByteData(0x06);
  delayMicroseconds(1);
  writeByteData(0xf0);
  delayMicroseconds(1);
  writeByteData(0x01);
  delayMicroseconds(1);
  writeByteData(0xf0);
  delayMicroseconds(1);
  writeByteData(0x00);
  delayMicroseconds(1);
  writeByteData(0x00);
  delayMicroseconds(1);
  writeCom(0xd0);
  delayMicroseconds(1);
  writeByteData(0x0d);
  delayMicroseconds(1);
  writeCom(0x2C);
  delayMicroseconds(1);
  setCs();
  setColor(255, 255, 255);
  setBackColor(0, 0, 0);

  int pageZone = config.width * 3; // 1963 5" only
  setScrollArea(0, pageZone, pageZone);

}

/*
  Page 1 = Lines 0-271
  Page 2 = Lines 272 - 543
  Page 3 = Lines 544 - 815
*/
void TeensyTFT::scroll(int y) {

  int16_t pix = y;
  clearCs();
  writeCom(0x37);
  writeByteData(pix >> 8);
  writeByteData(pix);
  setCs();
}

void TeensyTFT::setScrollArea(uint16_t top, uint16_t scr, uint16_t bottom) {

  clearCs();

  writeCom(0x33);
  writeByteData(top >> 8);
  writeByteData(top);
  writeByteData(scr >> 8);
  writeByteData(scr);
  writeByteData(bottom >> 8);
  writeByteData(bottom);


  writeComData(0x48, bottom);  //First window start
  writeComData(0x49, top);     //First window end

  writeComData(0x4A, bottom);  //Second window start
  writeComData(0x4B, top);     //Second window end

  writeComData(0x45, (uint16_t)bottom);  // Vertical RAM address start position
  writeComData(0x46, (uint16_t)top);     // Vertical RAM address end position

  setCs();
}

void TeensyTFT::setPage(int pageNumber) {
  scroll(pageNumber);
}

// not working for ssd1289 - load from sd or usb instead
void TeensyTFT::load_raw_image_mem(int x1, int y1, int imgX, int imgY, const uint16_t* img, int size, int page) {

  if (config.orient == PORTRAIT) {
    x1 = x1 + page;
  } else {
    y1 = y1 + page;
  }

  clearCs();

  setXY(x1, y1, (imgX + x1) - 1, (imgY + y1) - 1 );

  for (int y = 0; y < size; y++) {

    writeData((img[y] >> 8), img[y]);
  }

  setCs();
  clrXY();
}

/*
  x1 = X point
  y1 = Y poiint
  imgX = Image width
  imgY = Image height
  img = image data
  deg = rotations in degrees
  rox = rotation point x
  roy = rotation point y


*/
void TeensyTFT::load_raw_image_mem(int x1, int y1, int imgX, int imgY, const uint16_t *img, int size, int deg, int rox, int roy, int page) {

  int newx, newy, nx, ny;
  float  radian;
  radian = (PI / 180) * deg;

  if (deg == 0) {
    TeensyTFT::load_raw_image_mem(x1, y1, imgX, imgY, img, size, page);
  } else {


    clearCs();
    int ct = 0;
    for  (int y = 0; y < imgY; y++)
    {
      for (int x = 0; x < imgX; x++)
      {

        nx = (x1 + x);
        ny = (y1 + y) ;

        if (config.orient == PORTRAIT) {
          newx = (int) (cos(radian) * (nx - rox) - sin(radian) * (ny - roy) + rox) + page;
          newy = (int) (sin(radian) * (nx - rox) + cos(radian) * (ny - roy) + roy)  ;
        } else {
          newx = (int) (cos(radian) * (nx - rox) - sin(radian) * (ny - roy) + rox);
          newy = (int) (sin(radian) * (nx - rox) + cos(radian) * (ny - roy) + roy)  + page;

        }


        setXY(newx, newy, newx, newy);

        writeData((img[ct] >> 8), img[ct]);

        ct++;
      }

    }

    setCs();
    clrXY();
  }
}

void TeensyTFT::printText(uint16_t x, uint16_t y, const char* fmt, ...) {

  va_list arglist;
  char dest[MAXSTRINGSIZE];
  va_start(arglist, fmt);
  vsprintf(dest, fmt, arglist);
  va_end(arglist);
  print(dest, x, y);
}

void TeensyTFT::printTextPage(int page, uint16_t x, uint16_t y, const char* fmt, ... ) {
  va_list arglist;
  char dest[MAXSTRINGSIZE];
  va_start(arglist, fmt);
  vsprintf(dest, fmt, arglist);
  va_end(arglist);

  if (config.orient == PORTRAIT) {
    x = x + page;
  } else {
    y = y + page;
  }
  print(dest, x, y);
}

void TeensyTFT::fillScr(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t i;
  char ch, cl;

  ch = ((r & 248) | g >> 5);
  cl = ((g & 28) << 3 | b >> 3);
  clearCs();
  clrXY();
  uint32_t pixels = (config.width + 1) * (config.height + 1);
  setRs();

  for (i = 0; i < (pixels); i++)
  {
    writeBus(ch, cl);


  }
  setCs();


}

void TeensyTFT::fillPage(uint8_t r, uint8_t g, uint8_t b, int page) {

  uint32_t i;
  char ch, cl;

  ch = ((r & 248) | g >> 5);
  cl = ((g & 28) << 3 | b >> 3);

  clearCs();
  clrXY();
  uint32_t x1, x2, y1, y2;
  x1 = 0;
  y1 = page;

  x2 = config.height;
  y2 = config.width + page;

  if (config.orient == PORTRAIT) {
    swap(int, x1, y1);
    swap(int, x2, y2);
  }

  setXY(x1, y1, x2, y2);


  uint32_t pixels = ((config.width + 1) * (config.height + 1));

  setRs();

  for (i = 0; i < (pixels); i++) {
    writeBus(ch, cl);

  }
  setCs();

}

void TeensyTFT::drawCircle(int x, int y, int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;
  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();//clearBit(CS);
  setXY(x, y + radius, x, y + radius);
  writeData(ch, cl);
  setXY(x, y - radius, x, y - radius);
  writeData(ch, cl);
  setXY(x + radius, y, x + radius, y);
  writeData(ch, cl);
  setXY(x - radius, y, x - radius, y);
  writeData(ch, cl);

  while (x1 < y1)
  {
    if (f >= 0)
    {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;
    setXY(x + x1, y + y1, x + x1, y + y1);
    writeData(ch, cl);
    setXY(x - x1, y + y1, x - x1, y + y1);
    writeData(ch, cl);
    setXY(x + x1, y - y1, x + x1, y - y1);
    writeData(ch, cl);
    setXY(x - x1, y - y1, x - x1, y - y1);
    writeData(ch, cl);
    setXY(x + y1, y + x1, x + y1, y + x1);
    writeData(ch, cl);
    setXY(x - y1, y + x1, x - y1, y + x1);
    writeData(ch, cl);
    setXY(x + y1, y - x1, x + y1, y - x1);
    writeData(ch, cl);
    setXY(x - y1, y - x1, x - y1, y - x1);
    writeData(ch, cl);
  }
  setCs();//setBit(CS);
  clrXY();
}

void TeensyTFT::drawCircle(int x, int y, int radius, int page)
{

  if (config.orient == PORTRAIT) {
    swap(int, x, y);
    x = x + page;
  } else {
    y = y + page;
  }


  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;
  char ch, cl;

  ch = ((fcolorr & 248) | fcolorg >> 5);
  cl = ((fcolorg & 28) << 3 | fcolorb >> 3);

  clearCs();//clearBit(CS);
  setXY(x, y + radius, x, y + radius);
  writeData(ch, cl);
  setXY(x, y - radius, x, y - radius);
  writeData(ch, cl);
  setXY(x + radius, y , x + radius, y);
  writeData(ch, cl);
  setXY(x - radius, y, x - radius, y);
  writeData(ch, cl);

  while (x1 < y1)
  {
    if (f >= 0)
    {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;
    setXY(x + x1, y + y1, x + x1, y + y1);
    writeData(ch, cl);
    setXY(x - x1, y + y1, x - x1, y + y1);
    writeData(ch, cl);
    setXY(x + x1, y - y1, x + x1, y - y1);
    writeData(ch, cl);
    setXY(x - x1, y - y1, x - x1, y - y1);
    writeData(ch, cl);
    setXY(x + y1, y + x1, x + y1, y + x1);
    writeData(ch, cl);
    setXY(x - y1, y + x1, x - y1, y + x1);
    writeData(ch, cl);
    setXY(x + y1, y - x1, x + y1, y - x1);
    writeData(ch, cl);
    setXY(x - y1, y - x1, x - y1, y - x1);
    writeData(ch, cl);
  }
  setCs();
  clrXY();
}



void TeensyTFT::fillCircle(int x, int y, int radius)
{
  clearCs();
  for (int y1 = -radius; y1 <= radius; y1++)
    for (int x1 = -radius; x1 <= radius; x1++)
      if (x1 * x1 + y1 * y1 <= radius * radius)
      {
        setXY(x + x1, y + y1, x + x1, y + y1);
        writeData(((fcolorr & 248) | fcolorg >> 5), ((fcolorg & 28) << 3 | fcolorb >> 3));
      }
  setCs();
  clrXY();
}

void TeensyTFT::fillCircle(int x, int y, int radius, int page)
{
  if (config.orient == PORTRAIT) {
    swap(int, x, y);
    x = x + page;
  } else {
    y = y + page;
  }

  clearCs();
  for (int y1 = -radius; y1 <= radius; y1++)
    for (int x1 = -radius; x1 <= radius; x1++)
      if (x1 * x1 + y1 * y1 <= radius * radius)
      {
        setXY(x + x1, y + y1, x + x1, y + y1);
        writeData(((fcolorr & 248) | fcolorg >> 5), ((fcolorg & 28) << 3 | fcolorb >> 3));
      }
  setCs();
  clrXY();
}


void TeensyTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{

  if (x1 > x2)
  {
    swap(int, x1, x2);
  }
  if (y1 > y2)
  {
    swap(int, y1, y2);
  }
  if ((x2 - x1) > 4 && (y2 - y1) > 4)
  {
    drawPixel(x1 + 1, y1 + 1);
    drawPixel(x2 - 1, y1 + 1);
    drawPixel(x1 + 1, y2 - 1);
    drawPixel(x2 - 1, y2 - 1);
    drawHLine(x1 + 2, y1, x2 - x1 - 4);
    drawHLine(x1 + 2, y2, x2 - x1 - 4);
    drawVLine(x1, y1 + 2, y2 - y1 - 4);
    drawVLine(x2, y1 + 2, y2 - y1 - 4);
  }
}

void TeensyTFT::drawRoundRect(int x1, int y1, int x2, int y2, int page)
{

  if (config.orient == PORTRAIT) {
    x1 = x1 + page;
    x2 = x2 + page;

  } else {
    y1 = y1 + page;
    y2 = y2 + page;

  }

  if (x1 > x2)
  {
    swap(int, x1, x2);

  }
  if (y1 > y2)
  {
    swap(int, y1, y2);

  }
  if ((x2 - x1) > 4 && (y2 - y1) > 4)
  {
    drawPixel(x1 + 1, y1 + 1);
    drawPixel(x2 - 1, y1 + 1);
    drawPixel(x1 + 1, y2 - 1);
    drawPixel(x2 - 1, y2 - 1);
    drawHLine(x1 + 2, y1, x2 - x1 - 4);
    drawHLine(x1 + 2, y2, x2 - x1 - 4);
    drawVLine(x1, y1 + 2, y2 - y1 - 4);
    drawVLine(x2, y1 + 2, y2 - y1 - 4);
  }
}

void TeensyTFT::drawPixel(int x, int y)
{
  clearCs();
  setXY(x, y, x, y);
  setPixel(fcolorr, fcolorg, fcolorb);
  setCs();//setBit(CS);
  clrXY();
}

void TeensyTFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
  if (x1 > x2)
  {
    swap(int, x1, x2);
  }
  if (y1 > y2)
  {
    swap(int, y1, y2);
  }

  if ((x2 - x1) > 4 && (y2 - y1) > 4)
  {
    for (int i = 0; i < ((y2 - y1) / 2) + 1; i++)
    {
      switch (i)
      {
        case 0:
          drawHLine(x1 + 2, y1 + i, x2 - x1 - 4);
          drawHLine(x1 + 2, y2 - i, x2 - x1 - 4);
          break;
        case 1:
          drawHLine(x1 + 1, y1 + i, x2 - x1 - 2);
          drawHLine(x1 + 1, y2 - i, x2 - x1 - 2);
          break;
        default:
          drawHLine(x1, y1 + i, x2 - x1);
          drawHLine(x1, y2 - i, x2 - x1);
      }
    }
  }

}

void TeensyTFT::fillRoundRect(int x1, int y1, int x2, int y2, int page)
{

  if (config.orient == PORTRAIT) {

    x1 = x1 + page;
    x2 = x2 + page;

  } else {
    y1 = y1 + page;
    y2 = y2 + page;

  }


  if (x1 > x2)
  {
    swap(int, x1, x2);
  }
  if (y1 > y2)
  {
    swap(int, y1, y2);
  }

  if ((x2 - x1) > 4 && (y2 - y1) > 4)
  {
    for (int i = 0; i < ((y2 - y1) / 2) + 1; i++)
    {
      switch (i)
      {
        case 0:
          drawHLine(x1 + 2, y1 + i, x2 - x1 - 4);
          drawHLine(x1 + 2, y2 - i, x2 - x1 - 4);
          break;
        case 1:
          drawHLine(x1 + 1, y1 + i, x2 - x1 - 2);
          drawHLine(x1 + 1, y2 - i, x2 - x1 - 2);
          break;
        default:
          drawHLine(x1, y1 + i, x2 - x1);
          drawHLine(x1, y2 - i, x2 - x1);
      }
    }
  }

}


void TeensyTFT::fillRec(int x1, int y1, int x2, int y2, int page) {

  if (config.orient == PORTRAIT) {

    x1 = x1 + page;
    x2 = x2 + page;

  } else {
    y1 = y1 + page;
    y2 = y2 + page;

  }
  if (x1 > x2)
  {
    swap(int, x1, x2);
  }
  if (y1 > y2)
  {
    swap(int, y1, y2);
  }

  if (config.orient == PORTRAIT)
  {
    for (int i = 0; i < ((y2 - y1) / 2) + 1; i++)
    {
      drawHLine(x1, y1 + i, x2 - x1);
      drawHLine(x1, y2 - i, x2 - x1);
    }
  }
  else
  {
    for (int i = 0; i < ((x2 - x1) / 2) + 1; i++)
    {
      drawVLine(x1 + i, y1, y2 - y1);
      drawVLine(x2 - i, y1 , y2 - y1);
    }
  }

}

void TeensyTFT::fillRec(int x1, int y1, int x2, int y2)
{

  if (x1 > x2)
  {
    swap(int, x1, x2);
  }
  if (y1 > y2)
  {
    swap(int, y1, y2);
  }

  if (config.orient == PORTRAIT)
  {
    for (int i = 0; i < ((y2 - y1) / 2) + 1; i++)
    {
      drawHLine(x1, y1 + i, x2 - x1);
      drawHLine(x1, y2 - i, x2 - x1);
    }
  }
  else
  {
    for (int i = 0; i < ((x2 - x1) / 2) + 1; i++)
    {
      drawVLine(x1 + i, y1, y2 - y1);
      drawVLine(x2 - i, y1, y2 - y1);
    }
  }
}
