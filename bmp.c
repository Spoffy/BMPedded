#include <stdlib.h>
#include "bmp.h"

/* Load in the headers and data necessary to start retrieving image data. */
/* Grossly violates strict aliasing, so compile without it. */
status_t init_bmp(bmp_image_loader_state * loaderState, bmp_need_more_bytes dataRetrievalFunc)
{
  bmp_data_request data_request;

  loaderState->data_request_func = dataRetrievalFunc;

  data_request.buffer = (void *)&loaderState->fileHeader;
  data_request.bufferSize = sizeof(bmp_file_header);
  data_request.readType = READ;

  dataRetrievalFunc(&data_request);

  /* For now, only support BMPINFOHEADER, as CORE doesn't have 16 bit color. */
  data_request.buffer = (void *)&loaderState->dibHeader;
  data_request.bufferSize = sizeof(bmp_info_header);
  dataRetrievalFunc(&data_request);

  /* Skip to the image data */
  /* Untested, could cause problem */
  data_request.bufferSize = loaderState->fileHeader.imageDataOffset - (sizeof(bmp_file_header) + sizeof(bmp_info_header));
  data_request.readType = SKIP;
  dataRetrievalFunc(&data_request);

  /* Weird formula because BMP row sizes are padded up to a multiple of 4 bytes. */
  loaderState->rowSize = ((loaderState->dibHeader.bitsPerPixel * loaderState->dibHeader.imageWidth + 31) / 32) * 4;
  loaderState->imageDataRow = (uint16_t *)malloc(loaderState->rowSize);
  loaderState->currentRow = 0;
  return STATUS_OK;
}

status_t bmp_next_row(bmp_image_loader_state * loaderState) {
  bmp_data_request data_request;
  data_request.buffer = (void*)loaderState->imageDataRow;
  data_request.bufferSize = loaderState->rowSize;
  data_request.readType = READ;
  loaderState->data_request_func(&data_request);
  loaderState->currentRow += 1;
  return STATUS_OK;
}
