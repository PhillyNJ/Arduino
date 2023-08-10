#include "TeensyTFT.h"
#include <stdarg.h>
#include <stdio.h>

TeensyTFT::TeensyTFT() {
  // constructor
}

void TeensyTFT::initTFT(struct tft_chip tftConfig) {

  printf("Setting config\n\r");
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

  Serial.println("Setting pin I/O");

  switch (config.tft_model) {

    case ILI9325D_8:
    case ILI9341:
      // to do
      break;
    case ILI9325D_16:
    case SSD1963_480:
    case SSD1963_800:
    case SSD1963_800_5IN:
    case SSD1289:
      GPIO6_GDIR = GPIO6_MASK;
      GPIO7_GDIR = GPIO7_MASK;
      GPIO8_GDIR = GPIO8_MASK;
      GPIO9_GDIR = GPIO9_MASK;
      Serial.println("16 Bit configuration");
      break;
    default:
      Serial.println("Device not recognized\n\r");
      break;
  }

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

  if(config.tft_model == ILI9325D_16 ){
    delayMicroseconds(1);
    
  }
  if( config.tft_model == SSD1289){
    delayMicroseconds(1);

  }
  GPIO6_DR_SET = (1 << (WR));    // set bit

}

void TeensyTFT::writeBus(uint8_t hv, uint8_t lv) {

  switch (config.tft_model) {

    case ILI9325D_8:
    case ILI9341:
      //sam_writeToTFT(hv);
      //sam_pulseBitLow();
      //sam_writeToTFT(lv);
      //sam_pulseBitLow();
      Serial.println("Device not supported yet\n\r");
      break;
    case ILI9325D_16:
    case SSD1963_480:
    case SSD1963_800:
    case SSD1963_800_5IN:
    case SSD1289:
      write16ToTFT(hv, lv);
      pulseBitLow();
      
      break;
    default:
      Serial.println("Device not recognized\n\r");
      break;
  }

}

void TeensyTFT::writeData(uint8_t vh, uint8_t vl) {

  setRs();

  switch (config.tft_model) {

    case ILI9325D_8:
    case ILI9341:
      printf("Device not supported yet\n\r");
      //sam_writeBus(vh, vl);
      break;
    case ILI9325D_16:
    case SSD1963_480:
    case SSD1963_800:
    case SSD1963_800_5IN:
    case SSD1289:
      write16ToTFT(vh, vl);
      pulseBitLow();
      break;
    default:
      printf("Device not recognized\n\r");
      break;
  }
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


  switch (config.tft_model) {

    case ILI9325D_8:
    case ILI9341:
      // to do
      break;
    case ILI9325D_16:
    
      writeComData(0x20, x1);
      writeComData(0x21, y1);
      writeComData(0x50, x1);
      writeComData(0x52, y1);
      writeComData(0x51, x2);
      writeComData(0x53, y2);
      writeCom(0x22);
      break;
    case SSD1963_480:
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

      break;
    case SSD1963_800:
    case SSD1963_800_5IN:

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
      break;
    case SSD1289:

      writeComData(0x44, (x2 << 8) + x1);
      writeComData(0x45, y1);
      writeComData(0x46, y2);
      writeComData(0x4e, x1);
      writeComData(0x4f, y1);
      writeCom(0x22);
      break;
    default:
      Serial.println("Device not recognized\n\r");
      break;
  }


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
  Serial.println("Setting TFT");
  int pageZone = 0;
  switch (config.tft_model) {

    case ILI9325D_8:
    case ILI9341:
      // to do
      break;
    case ILI9325D_16:
    
      writeComData(0xE5, 0x78F0);
      delayMicroseconds(1);
      writeComData(0x01, 0x0100);
      delayMicroseconds(1);
      writeComData(0x02, 0x0200); // set 1 line inversion
      delayMicroseconds(1);
      writeComData(0x03, 0x1030); // set GRAM write direction and BGR=1.
      delayMicroseconds(1);
      writeComData(0x04, 0x0000); // Resize register
      delayMicroseconds(1);
      writeComData(0x08, 0x0207); // set the back porch and front porch
      delayMicroseconds(1);
      writeComData(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
      delayMicroseconds(1);
      writeComData(0x0A, 0x0000); // FMARK function
      delayMicroseconds(1);
      writeComData(0x0C, 0x0000); // RGB interface setting
      delayMicroseconds(1);
      writeComData(0x0D, 0x0000); // Frame marker Position
      delayMicroseconds(1);
      writeComData(0x0F, 0x0000); // RGB interface polarity
      delayMicroseconds(1);
      //*************Power On sequence ****************//
      writeComData(0x10, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
      delayMicroseconds(1);
      writeComData(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
      delayMicroseconds(1);
      writeComData(0x12, 0x0000); // VREG1OUT voltage
      delayMicroseconds(1);
      writeComData(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
      delayMicroseconds(1);
      writeComData(0x07, 0x0001);
      delayMicroseconds(50);
      // delay(200); // Dis-charge capacitor power voltage
      writeComData(0x10, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
      delayMicroseconds(1);
      writeComData(0x11, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
      delayMicroseconds(50);
      // delay(50); // Delay 50ms
      writeComData(0x12, 0x000D); // 0012
      delayMicroseconds(50);
      //  delay(50); // Delay 50ms
      writeComData(0x13, 0x1200); // VDV[4:0] for VCOM amplitude
      delayMicroseconds(1);
      writeComData(0x29, 0x000A); // 04  VCM[5:0] for VCOMH
      delayMicroseconds(1);
      writeComData(0x2B, 0x000D); // Set Frame Rate
      delayMicroseconds(150);
      // delay(50); // Delay 50ms
      writeComData(0x20, 0x0000); // GRAM horizontal Address
      delayMicroseconds(1);
      writeComData(0x21, 0x0000); // GRAM Vertical Address
      delayMicroseconds(1);
      // ----------- Adjust the Gamma Curve ----------//
      writeComData(0x30, 0x0000);
      delayMicroseconds(1);
      writeComData(0x31, 0x0404);
      delayMicroseconds(1);
      writeComData(0x32, 0x0003);
      delayMicroseconds(1);
      writeComData(0x35, 0x0405);
      delayMicroseconds(1);
      writeComData(0x36, 0x0808);
      delayMicroseconds(1);
      writeComData(0x37, 0x0407);
      delayMicroseconds(1);
      writeComData(0x38, 0x0303);
      delayMicroseconds(1);
      writeComData(0x39, 0x0707);
      delayMicroseconds(1);
      writeComData(0x3C, 0x0504);
      delayMicroseconds(1);
      writeComData(0x3D, 0x0808);
      delayMicroseconds(1);
      //------------------ Set GRAM area ---------------//
      writeComData(0x50, 0x0000); // Horizontal GRAM Start Address
      delayMicroseconds(1);
      writeComData(0x51, 0x00EF); // Horizontal GRAM End Address
      delayMicroseconds(1);
      writeComData(0x52, 0x0000); // Vertical GRAM Start Address
      delayMicroseconds(1);
      writeComData(0x53, 0x013F); // Vertical GRAM Start Address
      delayMicroseconds(1);
      writeComData(0x60, 0xA700); // Gate Scan Line
      delayMicroseconds(1);
      writeComData(0x61, 0x0001); // NDL,VLE, REV
      delayMicroseconds(1);
      writeComData(0x6A, 0x0000); // set scrolling line
      delayMicroseconds(1);
      //-------------- Partial Display Control ---------//
      writeComData(0x80, 0x0000);
      delayMicroseconds(1);
      writeComData(0x81, 0x0000);
      delayMicroseconds(1);
      writeComData(0x82, 0x0000);
      delayMicroseconds(1);
      writeComData(0x83, 0x0000);
      delayMicroseconds(1);
      writeComData(0x84, 0x0000);
      delayMicroseconds(1);
      writeComData(0x85, 0x0000);
      delayMicroseconds(1);
      //-------------- Panel Control -------------------//
      writeComData(0x90, 0x0010);
      delayMicroseconds(1);
      writeComData(0x92, 0x0000);
      delayMicroseconds(1);
      writeComData(0x07, 0x0133); // 262K color and display ON
      delayMicroseconds(1);
      break;
    case SSD1963_480:

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

      pageZone = config.width * 3; // 1963 5" only
      setScrollArea(0, pageZone, pageZone);
      break;
    case SSD1963_800:
      Serial.println("Not support yet");
      break;
    case SSD1963_800_5IN:
      writeCom(0xE2);        //PLL multiplier, set PLL clock to 120M
      delayMicroseconds(1);
      writeByteData(0x23);      //N=0x36 for 6.5M, 0x23 for 10M crystal
      delayMicroseconds(1);
      writeByteData(0x02);
      delayMicroseconds(1);
      writeByteData(0x04);
      delayMicroseconds(1);
      writeCom(0xE0);       // PLL enable
      delayMicroseconds(1);
      writeByteData(0x01);
      delayMicroseconds(10);
      writeCom(0xE0);
      delayMicroseconds(1);
      writeByteData(0x03);
      delayMicroseconds(10);

      writeCom(0x01);       // software reset
      delayMicroseconds(100);
      writeCom(0xE6);       //PLL setting for PCLK, depends on resolution
      delayMicroseconds(1);
      writeByteData(0x04);
      delayMicroseconds(1);
      writeByteData(0x93);
      delayMicroseconds(1);
      writeByteData(0xE0);
      delayMicroseconds(1);
      writeCom(0xB0);       //LCD SPECIFICATION
      delayMicroseconds(1);
      writeByteData(0x20);
      delayMicroseconds(1);
      writeByteData(0x00);
      delayMicroseconds(1);
      writeByteData(0x03);    //Set HDP 799
      delayMicroseconds(1);
      writeByteData(0x1F);
      delayMicroseconds(1);
      writeByteData(0x01);    //Set VDP 479
      delayMicroseconds(1);
      writeByteData(0xDF);
      delayMicroseconds(1);
      writeByteData(0x00);
      delayMicroseconds(1);
      writeCom(0xB4);       //HSYNC
      delayMicroseconds(1);
      writeByteData(0x03);    //Set HT  928
      delayMicroseconds(1);
      writeByteData(0xA0);
      delayMicroseconds(1);
      writeByteData(0x00);    //Set HPS 46
      delayMicroseconds(1);
      writeByteData(0x2E);
      delayMicroseconds(1);
      writeByteData(0x30);    //Set HPW 48
      delayMicroseconds(1);
      writeByteData(0x00);    //Set LPS 15
      delayMicroseconds(1);
      writeByteData(0x0F);
      delayMicroseconds(1);
      writeByteData(0x00);
      delayMicroseconds(1);
      writeCom(0xB6);       //VSYNC
      delayMicroseconds(1);
      writeByteData(0x02);    //Set VT  525
      delayMicroseconds(1);
      writeByteData(0x0D);
      delayMicroseconds(1);
      writeByteData(0x00);    //Set VPS 16
      delayMicroseconds(1);
      writeByteData(0x10);
      delayMicroseconds(1);
      writeByteData(0x10);    //Set VPW 16
      delayMicroseconds(1);
      writeByteData(0x00);    //Set FPS 8
      delayMicroseconds(1);
      writeByteData(0x08);
      delayMicroseconds(1);
      writeCom(0xBA);
      delayMicroseconds(1);
      writeByteData(0x05);    //GPIO[3:0] out 1
      delayMicroseconds(1);
      writeCom(0xB8);
      delayMicroseconds(1);
      writeByteData(0x07);      //GPIO3=input, GPIO[2:0]=output
      delayMicroseconds(1);
      writeByteData(0x01);    //GPIO0 normal
      delayMicroseconds(1);
      writeCom(0x36);       //rotation
      delayMicroseconds(1);
      writeByteData(0x21);    // use 0x21 or 0x22
      delayMicroseconds(1);
      writeCom(0xF0);       //pixel data interface
      delayMicroseconds(1);
      writeByteData(0x03);
      delayMicroseconds(1);
      setXY(0, 0, 799, 479);
      writeCom(0x29);   //display on
      delayMicroseconds(1);
      writeCom(0xBE);   //set PWM for B/L
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
      pageZone = config.width;
      setScrollArea(0, pageZone, pageZone);
      break;
    case SSD1289:

      writeComData(0x00, 0x0001); //Oscillation Start
      delay(2);
      writeComData(0x03, 0xA8A4); //Power control (1)
      delay(2);;
      writeComData(0x0C, 0x0000); //Power control (2)
      delay(2);;
      writeComData(0x0D, 0x080C); //Power control (3)
      delay(2);
      writeComData(0x0E, 0x2B00); //Power control (4)
      delay(2);
      writeComData(0x1E, 0x00B7); //Power control (5)
      delay(2);
      writeComData(0x01, 0x2B3F); //Driver output control
      delay(2);
      writeComData(0x02, 0x0600); //LCD drive AC control
      delay(2);
      writeComData(0x10, 0x0000); //Sleep mode
      delay(2);
      writeComData(0x11, 0x6070); //Entry mode
      delay(2);
      writeComData(0x05, 0x0000); //Compare register(1)
      delay(2);
      writeComData(0x06, 0x0000); //Compare register(2)
      delay(2);
      writeComData(0x16, 0xEF1C); //Horizontal Porch
      delay(2);
      writeComData(0x17, 0x0003); //Vertical Porch
      delay(2);
      writeComData(0x07, 0x0633); //Display control was 0x233
      delay(2);
      writeComData(0x0B, 0x0000); //Frame cycle control
      delay(2);
      writeComData(0x0F, 0x0000); //Gate scan start position
      delay(2);
      writeComData(0x41, 0x0000); //Vertical scroll control (1)
      delay(2);
      writeComData(0x42, 0x0000); //Vertical scroll control (2)
      delay(2);
      writeComData(0x48, 0x0000); //First window start
      delay(2);
      writeComData(0x49, 0x013F); //First window end
      delay(2);
      writeComData(0x4A, 0x0000); //Second window start
      delay(2);
      writeComData(0x4B, 0x0000); //Second window end
      delay(2);
      writeComData(0x44, 0xEF00); // Horizontal RAM address position
      delay(2);
      writeComData(0x45, 0x0000); // Vertical RAM address start position
      delay(2);
      writeComData(0x46, 0x013F); // Vertical RAM address end position
      delay(2);
      writeComData(0x30, 0x0707); // ? control (1)
      delay(2);
      writeComData(0x31, 0x0204); // ? control (2)
      delay(2);
      writeComData(0x32, 0x0204);
      delay(2);
      writeComData(0x33, 0x0502);
      delay(2);
      writeComData(0x34, 0x0507);
      delay(2);
      writeComData(0x35, 0x0204);
      delay(2);
      writeComData(0x36, 0x0204);
      delay(2);
      writeComData(0x37, 0x0502);
      delay(2);
      writeComData(0x3A, 0x0302);
      delay(2);
      writeComData(0x3B, 0x0302); //? control (10)
      delay(2);
      writeComData(0x23, 0x0000); // RAM write data mask (1)
      delay(2);
      writeComData(0x24, 0x0000); // RAM write data mask (2)
      delay(2);
      //writeComData(0x25,0x8000); //?
      writeComData(0x4f, 0x0000); // Set GDDRAM Y address counter
      delay(2);
      writeComData(0x4e, 0x0000); // Set GDDRAM X address counter
      delay(2);
      writeCom(0x22);
      delay(2);
      
      setColor(255, 255, 255);
      setBackColor(0, 0, 0);
     
      break;
    default:
      Serial.println("Device not recognized\n\r");
      break;
  }


}

/* SSD1963_480
  Page 1 = Lines 0-271
  Page 2 = Lines 272 - 543
  Page 3 = Lines 544 - 815
*/
void TeensyTFT::scroll(int y) {

  int16_t pix = y;
  clearCs();

  if(config.tft_model == ILI9325D_8 || config.tft_model == ILI9325D_16){
  
    writeComData(0x61,0x0003);
    writeComData(0x6A, pix);
  
  }else if(config.tft_model == SSD1963_480 || config.tft_model == SSD1963_800 || config.tft_model == SSD1963_800_5IN){
       
    writeCom(0x37);
    writeByteData(pix>>8);
    writeByteData(pix);
     

  } else if (config.tft_model == SSD1289){
      
    writeComData(0x41, y); // screen 1 scrolls    
    writeComData(0x42, y);// screen 2 scroll
    
  } else {
    
    printf("Scroll not supported \n\r");
    
  }
  
  setCs();

/*
 writeCom(0x37);
  writeByteData(pix >> 8);
  writeByteData(pix);
  setCs();
*/
 
}

void TeensyTFT::setScrollArea(uint16_t top, uint16_t scr, uint16_t bottom) {

  clearCs();

  if (config.tft_model == SSD1963_480 || config.tft_model == SSD1963_800 || config.tft_model == SSD1963_800_5IN) {

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
  } else if(config.tft_model == SSD1289){

    if(bottom < 0){
     bottom = 0;
    }
    if(top > 320){
      
      top = 320;
    }

    writeComData(0x48,bottom); //First window start
    writeComData(0x49,top); //First window end

    writeComData(0x4A,bottom); //Second window start
    writeComData(0x4B,top); //Second window end

    writeComData(0x45,bottom); // Vertical RAM address start position
    writeComData(0x46,top); // Vertical RAM address end position
   
  }


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


  uint32_t pixels = ((config.width ) * (config.height));

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
