#ifndef BMP_LCD_HEADER
#define BMP_LCD_HEADER

#include "bmp.h"

status_t display_segment_bmp(uint16_t x, uint16_t y, rectangle * area, bmp_image_loader_state * loaderState);
#endif
