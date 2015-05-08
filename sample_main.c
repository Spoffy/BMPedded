/* COMP2215 Task 6  Model Answer */

#include "os.h"
#include "bmp.h"
#include "bmp_lcd.h"


/* Lines have to be shorter than this. Note: buffer is on stack. */
#define LINE_BUFF_LEN 55    
//#define IMAGE_CACHE_SIZE 2048

FIL File;  						/* FAT File */

void read_image_bytes(bmp_data_request * request)
{
    UINT temp = 0;
    f_lseek(&File, request->dataOffset);
    f_read(&File, request->buffer, request->bufferSize, &temp);
}

void main(void) {
    os_init();

    bmp_image_loader_state imageState;
    rectangle rect = {0, 320, 0, 240};

		f_mount(&FatFs, "", 0);
		f_open(&File,"tuxsimp", FA_READ);	
    status_t stat = init_bmp(&imageState, read_image_bytes, 4096);
    if(stat == STATUS_OK) {
      rect.right = imageState.dibHeader.imageWidth;
      rect.bottom = imageState.dibHeader.imageHeight;
      display_segment_bmp(0, 0, &rect, &imageState);
    }
		f_close(&File);

}


