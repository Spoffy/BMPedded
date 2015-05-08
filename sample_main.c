/* COMP2215 Task 6  Model Answer */

#include "os.h"
#include "bmp.h"
#include "bmp_lcd.h"


/* Lines have to be shorter than this. Note: buffer is on stack. */
#define LINE_BUFF_LEN 55    

FIL File;  						/* FAT File */

int position = 0;

void read_image_bytes(bmp_data_request * request)
{
  UINT bytes_read = 0;
  f_lseek(&File, request->dataOffset);
  f_read(&File, request->buffer, request->bufferSize, &bytes_read);
}

void main(void) {
    os_init();

    bmp_image_loader_state imageState;
    rectangle rect = {0, 120, 0, 60};

		f_mount(&FatFs, "", 0);
		f_open(&File,"tuxsimp", FA_READ);	
    init_bmp(&imageState, read_image_bytes);
    rect.right = imageState.dibHeader.imageWidth;
    rect.bottom = imageState.dibHeader.imageHeight;
    display_segment_bmp(30, 0, &rect, &imageState);
		f_close(&File);

}


