#include "bmp_lcd.h"
#include "ili934x.h"

/* Draws a row of the imagei in reverse (because BMPs store their images upside down. */
inline void draw_row_bmp(uint16_t rowWidth, uint16_t * data)
{
    /* Potentially slightly faster unrolled, but almost unnoticably so. */
    uint16_t rowPos;
    for(rowPos = 0; rowPos < rowWidth; rowPos++) 
    {
      write_data16(data[rowPos]);
    }
}

inline void init_draw(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    write_cmd(COLUMN_ADDRESS_SET);
    write_data16(startX);
    write_data16(endX);
    write_cmd(PAGE_ADDRESS_SET);
    write_data16(startY);
    write_data16(endY);
    write_cmd(MEMORY_WRITE);
}

/*
Displays a portion of the BMP (specified by area) at coordinates x, y.
For example, with an area with left = 0, top = 0, right = 10, bottom = 10,
it'd display the top left 10x10 square of image at coordinates x,y. 

Loader should be an initialised image_loader.
*/

status_t display_segment_bmp(uint16_t x, uint16_t y, rectangle * area, bmp_image_loader_state * loaderState)
{
  loaderState->currentRow = area->top;
  uint16_t rowWidth = (area->right - area->left);
  uint16_t targetY = y + area->bottom;
  init_draw(x, y, x+rowWidth-1, targetY-1);
  while(y < targetY)
  {
    bmp_next_row(loaderState);
    draw_row_bmp(rowWidth, loaderState->imageDataRow+area->left);
    y++;
  }
  return STATUS_OK;
}
