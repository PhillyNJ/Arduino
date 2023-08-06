// Example used SD card.
#include "TeensyTFT.h"
#include "BitmapHandler.h"
#include "character_map.h"
#include "segoe_12pts.h"
#include "oil_pressure.h"
#include "needle.h"

int currentPage, bufferPage;
void togglePage();
void testBitMapsSd();
void testVline();
void testHLine();
void testImages();
void testCircles();
void testRectangles();
void clearPages(bool withLabels);

const char *images[] = { "splash.bmp", "c172c-1.bmp", "c172c-2.bmp",  "panel.bmp", "Airspeed_Background.bmp", "chart.bmp"};

const int chipSelect = BUILTIN_SDCARD;

struct tft_chip tft;

TeensyTFT myTft;

BitmapHandler bmh;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.pages = 3;
  tft.width = 272;
  tft.height = 480;

  tft.page_1 = 0;
  tft.page_2 = 272;
  tft.page_3 = 544;
  tft.tft_model = SSD1963_480;
  // tft.orient = PORTRAIT;
  tft.orient = LANDSCAPE;
  myTft.initTFT(tft);

  bmh.init(&myTft, tft.orient);

  myTft.fillScr(0, 0, 0);
  myTft.setCharMap(charMap);
  myTft.setBackColor(0, 0, 0);
  myTft.setColor(255, 255, 255);
  myTft.setFont(segoeUI_12ptBitmaps, segoeUI_12ptDescriptors, sizeof charMap);

  Serial.print("Initializing SD card...");
  myTft.printText(1, 1, "Init SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  myTft.printText(1, 20, "SD card complete", tft.page_1);
  myTft.printText(1, 40, "Start tests", tft.page_1);
  delay(1000);

  clearPages(true);
  testImages();
}

void loop() {


  testBitMapsSd();
  delay(1000);
  testImages();
  delay(1000);
  testCircles();
  delay(1000);
  testRectangles();
  delay(1000);


}
void testCircles() {

 clearPages(true);
  myTft.setPage(tft.page_1);

  for (int i = 110; i < 210; i += 10) {
    myTft.drawCircle(146, i, 50);
  }


  myTft.fillCircle(146, 110, 80);
  delay(1000);
  myTft.fillCircle(100, 110, 50,  tft.page_2);
  myTft.setPage(tft.page_2);
  delay(1000);
  myTft.fillCircle(150, 110, 50,  tft.page_3);
  myTft.setPage(tft.page_3);
  delay(1000);
  myTft.fillPage(0, 0, 0, tft.page_1);
  myTft.drawCircle(110, 210, 60, tft.page_1);
  myTft.setPage(tft.page_1);

  delay(1000);
  myTft.fillPage(0, 0, 0, tft.page_2);
  myTft.drawCircle(110, 150, 60, tft.page_2);
  myTft.setPage(tft.page_2);

  delay(1000);
  myTft.fillPage(0, 0, 0, tft.page_3);
  myTft.drawCircle(110, 150, 80, tft.page_3);
  myTft.setPage(tft.page_3);


}

void testImages() {

  clearPages(true);
  int cY = 0, cX = 0;
  myTft.setPage(tft.page_1);
  if (tft.orient == PORTRAIT) {

    // background is 240X240

    cY = (240 / 2);
    cX = (240 / 2) + 10;


    for (int i = 0; i < 45; i++) {
      myTft.setPage(currentPage);
      bmh.displayBmp(1, 20, images[4], bufferPage);
      myTft.load_raw_image_mem(cY, cX, 110, 15, needle, sizeof needle,  i, 122, 137, bufferPage);


      togglePage();
    }

  } else {

    cY = (tft.width / 2) - 5;

    for (int i = 0; i < 45; i++) {
      myTft.setPage(currentPage);

      bmh.displayBmp(1, 16, images[4], bufferPage);
      myTft.load_raw_image_mem(116, cY, 110, 15, needle, sizeof needle,  i, 122, 137, bufferPage);

      togglePage();

    }
  }



}

void testHline() {
  clearPages(true);

  myTft.setPage(tft.page_1);
  for (int i = 10; i < 200; i += 4) {
    myTft.drawHLinePage(20, i , i, tft.page_1);
    delayMicroseconds(100);
  }
  delay(1000);

  myTft.setPage(tft.page_2);
  for (int i = 10; i < 200; i += 4) {
    myTft.drawHLinePage(120, i , i, tft.page_2);
    delayMicroseconds(100);
  }
  delay(1000);

  myTft.setPage(tft.page_3);
  for (int i = 10; i < 200; i += 4) {
    myTft.drawHLinePage(50, i , i, tft.page_3);
    delayMicroseconds(100);
  }
  delay(1000);

}

void testVline() {
  // page 1
  clearPages(true);


  for (uint8_t i = 0; i < 200; i += 4) {
    myTft.drawVLine(20 + i, 100 , 40);
    delayMicroseconds(100);
  }
  delay(1000);
  //page 2
  myTft.setPage(tft.page_2);
  for (uint8_t i = 0; i < 200; i += 4) {
    myTft.drawVLinePage(20 + i, 120 , 40, tft.page_2);
    delayMicroseconds(100);
  }
  delay(1000);

  //page 3
  myTft.setPage(tft.page_3);
  for (uint8_t i = 0; i < 200; i += 4) {
    myTft.drawVLinePage(20 + i, 140 , 40, tft.page_3);
    delayMicroseconds(100);
  }
  delay(1000);


}

void testBitMapsSd() {
  clearPages(true);
  
  myTft.fillPage(0, 0, 0, tft.page_2);
  bmh.displayBmp(1, 1, images[0], tft.page_2);
  myTft.fillPage(0, 0, 0, tft.page_3);
  bmh.displayBmp(1, 1, images[1], tft.page_3);
  myTft.setPage(tft.page_3);
  delay(1000);
  bmh.displayBmp(1, 1, images[3], tft.page_1);
  delay(1000);
  myTft.setPage(tft.page_2);
  delay(1000);
  myTft.setPage(tft.page_1);
}

void togglePage() {
  if (currentPage ==  tft.page_2) {
    currentPage = tft.page_3;
    bufferPage = tft.page_2;
  } else {
    currentPage = tft.page_2;
    bufferPage = tft.page_3;
  }

}

void testRectangles() {

 clearPages(true);

  myTft.fillRec(20, 20, 100, 100);
  myTft.setPage(tft.page_1);

  myTft.fillRec(20, 20, 150, 150, tft.page_2);
  delay(1000);
  myTft.setPage(tft.page_2);

  myTft.fillRec(20, 20, 200, 200, tft.page_3);
  delay(1000);
  myTft.setPage(tft.page_3);
  delay(1000);
  myTft.fillPage(0, 0, 0, tft.page_1);
  myTft.fillPage(0, 0, 0, tft.page_2);
  myTft.fillPage(0, 0, 0, tft.page_3);

  myTft.drawRoundRect(20, 20, 270, 270);
  myTft.setPage(tft.page_1);

  delay(1000);
  myTft.drawRoundRect(20, 20, 200, 200, tft.page_2);
  myTft.setPage(tft.page_2);

  delay(1000);
  myTft.drawRoundRect(20, 20, 250, 250, tft.page_3);
  myTft.setPage(tft.page_3);

  delay(1000);

  myTft.fillPage(0, 0, 0, tft.page_1);
  myTft.fillPage(0, 0, 0, tft.page_2);
  myTft.fillPage(0, 0, 0, tft.page_3);

  myTft.fillRoundRect(20, 20, 270, 270, tft.page_1);
  myTft.setPage(tft.page_1);
  delay(1000);

  myTft.fillRoundRect(20, 20, 200, 200, tft.page_2);
  myTft.setPage(tft.page_2);
  delay(1000);

  myTft.fillRoundRect(20, 20, 250, 250, tft.page_3);
  myTft.setPage(tft.page_3);
  delay(1000);
}

void clearPages(bool withLabels) {

  myTft.fillPage(0, 0, 0, tft.page_1);
  myTft.fillPage(0, 0, 0, tft.page_2);
  myTft.fillPage(0, 0, 0, tft.page_3);

  if (withLabels) {
    myTft.printTextPage(tft.page_1, 1, 1 , "Page 1");
    myTft.printTextPage(tft.page_2, 1, 1 , "Page 2");
    myTft.printTextPage(tft.page_3, 1, 1 , "Page 3");
  }
}
