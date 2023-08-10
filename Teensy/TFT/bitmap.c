#include "bitmap.h"
#include <stdio.h>
 void print_bitmap_header(struct bitmap_t *bmp){

  printf("Offset:\t\t%hu\n\r", bmp->offset);
  printf("Height:\t\t%lu\n\r", bmp->height);
  printf("Width:\t\t%lu\n\r", bmp->width);
  printf("Row Size:\t%d\n\r", bmp->rowSize);
  printf("Offset:\t\t%hu\n\r", bmp->offset);
  printf("# of Pixels:\t%d\n\r", bmp->numberOfPixels);
  printf("Compression:\t%ld\n\r", bmp->compression);
  printf("Image Size:\t%ld\n\r", bmp->imageSize);
  printf("Name:\t\t%s\n\r", bmp->name);

 }
