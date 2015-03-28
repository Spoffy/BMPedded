#include "bmp_lcd.h"
#include "ili934x.h"

/* Draws a row of the imagei in reverse (because BMPs store their images upside down. */
inline void draw_row_bmp(uint16_t x, uint16_t y, uint16_t width, uint16_t * data)
{
    write_cmd(COLUMN_ADDRESS_SET);
    write_data16(x);
    write_data16(x+width);
    write_cmd(PAGE_ADDRESS_SET);
    write_data16(y);
    write_data16(y);
    write_cmd(MEMORY_WRITE);
    uint16_t rowPos;
    for(rowPos = 0; rowPos < width; rowPos++) 
    {
      write_data16(data[rowPos]);
    }
}

/*
Displays a portion of the BMP (specified by area) at coordinates x, y.
For example, with an area with left = 0, top = 0, right = 10, bottom = 10,
it'd display the top left 10x10 square of image at coordinates x,y. 

Loader should be an initialised image_loader.
*/

status_t display_segment_bmp(uint16_t x, uint16_t y, rectangle * area, bmp_image_loader_state * loaderState)
{
  loaderState->currentRow = loaderState->dibHeader.imageHeight - area->bottom;
  uint16_t rowWidth = (area->right - area->left);
  uint16_t currentY = y + (area->bottom - area->top);
  while(currentY > y)
  {
    bmp_next_row(loaderState);
    draw_row_bmp(x, currentY, rowWidth, (void*)(loaderState->imageDataRow+area->left));
    currentY--;
  }
  return STATUS_OK;
}
