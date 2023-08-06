#include "BitmapHandler.h"

BitmapHandler::BitmapHandler(){
  
};

void  BitmapHandler::init(TeensyTFT* tft, uint8_t _orient) {
  tftRef = tft;
  orient = _orient;
  Serial.println("Bitmap Handler");
}

void BitmapHandler::displayBmp(int x1, int y1, const char *bitmap, int pg) {

  if(orient == PORTRAIT){
    x1 = x1+pg;
  }else {
     y1 = y1+pg;
  }
  
  bmFile = SD.open(bitmap);
  
  if (bmFile) {
    
    Serial.println("Reading header first");

    // read from the file until there's nothing else in it:
    while (bmFile.available()) {

      //Serial.write(myFile.read());
      bmFile.read(header_buff, sizeof header_buff);
      image.signiture = (header_buff[1] << 8) | header_buff[0];
      if (image.signiture != 0x4d42) {

        Serial.println("File is now a bmp!");
        break;
      }

      uint16_t height = (header_buff[25] << 24) | (header_buff[24] << 16) | (header_buff[23] << 8) | header_buff[22];
      memcpy(&image.offset, &header_buff[6], sizeof(image.offset));
      memcpy(&image.height, &header_buff[22], sizeof(image.height));
      memcpy(&image.width, &header_buff[18], sizeof(image.width));
      memcpy(&image.offset, &header_buff[10], sizeof(image.offset)); //? bug?
      memcpy(&image.numberOfPixels, &header_buff[28], sizeof(image.numberOfPixels));
      memcpy(&image.compression, &header_buff[30], sizeof(image.compression));
      memcpy(&image.imageSize, &header_buff[34], sizeof(image.imageSize));
      image.rowSize = ((image.width * 3 + 3) & ~3);


      if (height > -1 ) {
        flip = false;
        position = bmFile.size() - image.rowSize;
        // start bottom up
      } else {

        flip = true;
        position = image.offset;
      }
      if (orient == LANDSCAPE) {
        swap(uint32_t, image.width, image.height);

      }

      uint8_t val = (image.height / image.numberOfPixels) % 4 ;
      if (val != 0) {
        padding  = 4 - val;
      }

      tftRef->clearCs();
      tftRef->setXY(x1, y1, (image.width + x1 - 1 ), (image.height + y1) - 1); // images must be 480X272 exactly to fit screen


      bool ret;
      Serial.println("Getting start of file...");
      ret = bmFile.seek(position);
      if (!ret) {
        Serial.println("Failed to set next position");
        return;
      }

      uint8_t rowBuff[image.rowSize];

      for (uint32_t row = 0; row < (image.imageSize / image.rowSize); row++) {
        ret = bmFile.read(rowBuff, sizeof rowBuff);
        if (!ret) {
          Serial.println("Failed to read row");
          return;
        }
        if (flip) {
          position = position + image.rowSize;
        } else {
          position = position - image.rowSize;

        }

        ret = bmFile.seek(position); // set next position
        if (!ret) {

          return;
        }

        if (!flip) {
          for (uint16_t x = 0; x < (sizeof rowBuff) - padding; x += 3) {
            tftRef->writeData(((rowBuff[x + 2] & 248) | rowBuff[x + 1] >> 5), ((rowBuff[x + 1] & 28 << 3) | rowBuff[x] >> 3));
          }

        } else {

          for (uint16_t x = (sizeof rowBuff) - padding; x > 0 ; x -= 3) {
            tftRef->writeData(((rowBuff[x + 2] & 248) | rowBuff[x + 1] >> 5), ((rowBuff[x + 1] & 28 << 3) | rowBuff[x] >> 3));
          }

        }

      }
    } // end while

    tftRef->setCs();
    tftRef->clrXY();


    // close the file:
    bmFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening logo.bmp");
  }
}
