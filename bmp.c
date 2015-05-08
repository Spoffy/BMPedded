#include <stdlib.h>
#include "bmp.h"

inline size_t calc_row_size(bmp_info_header * dibHeader) {
  /* Weird formula because BMP row sizes are padded up to a multiple of 4 bytes. */
  return (((dibHeader->bitsPerPixel * dibHeader->imageWidth) + 31) / 32) * 4;
}

/* Load in the headers and data necessary to start retrieving image data. */
/* Grossly violates strict aliasing, so compile without it. */
status_t init_bmp(bmp_image_loader_state * loaderState, bmp_need_more_bytes dataRetrievalFunc)
{
  bmp_data_request data_request;

  loaderState->data_request_func = dataRetrievalFunc;

  data_request.buffer = (void *)&loaderState->fileHeader;
  data_request.bufferSize = sizeof(bmp_file_header);
  data_request.dataOffset = 0;

  dataRetrievalFunc(&data_request);

  /* For now, only support BMPINFOHEADER, as CORE doesn't have 16 bit color. */
  data_request.buffer = (void *)&loaderState->dibHeader;
  data_request.bufferSize = sizeof(bmp_info_header);
  data_request.dataOffset = sizeof(bmp_file_header);
  dataRetrievalFunc(&data_request);

  loaderState->rowSize = calc_row_size(&loaderState->dibHeader);
  loaderState->endOfImage = (loaderState->fileHeader.imageDataOffset + (uint32_t)loaderState->rowSize * loaderState->dibHeader.imageHeight);
  loaderState->imageDataRow = (uint16_t *)malloc(loaderState->rowSize);
  loaderState->currentRow = 0;
  return STATUS_OK;
}

/* Loads the row specified by currentRow of the loader state into the buffer 
   And increments currentRow.
*/
status_t bmp_next_row(bmp_image_loader_state * loaderState) 
{
  bmp_data_request data_request;

  data_request.buffer = (void*)loaderState->imageDataRow;
  data_request.bufferSize = loaderState->rowSize;
  //First half are constant for a given file.
  data_request.dataOffset = loaderState->endOfImage 
                            - (uint32_t)loaderState->rowSize * loaderState->currentRow;

  loaderState->data_request_func(&data_request);
  loaderState->currentRow += 1;
  return STATUS_OK;
}

