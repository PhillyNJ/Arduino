/*
 * bitmap.h
 *
 * Created: 07/28/2023 
 *  Author: pvallone
 */ 

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef BITMAP_H_
#define BITMAP_H_
#include <stdint.h>



struct bitmap_t  {
  
  int32_t width;
  int32_t height; 
  uint16_t offset;
  uint16_t rowSize;
  uint8_t numberOfPixels;
  uint32_t compression;
  uint32_t imageSize;
  uint16_t signiture;
  char *name;
};

extern void print_bitmap_header(struct bitmap_t *bmp);

#endif /* BITMAP_H_ */

#ifdef  __cplusplus
}
#endif
