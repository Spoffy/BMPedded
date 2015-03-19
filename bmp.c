#include "bmp.h"

/** Load in the headers and data necessary to start retrieving image data. **/
uint8_t init_bmp(bmp_image_loader_state * loaderState, bmp_need_more_bytes dataRetrievalFunc)
{
  loaderState->data_request_func = dataRetrievalFunc;
  dataRetrievalFunc((void *)&loaderState->imageInfo.fileHeader, sizeof(bmp_file_header));
  /* Really ought to be tested it's loading correctly. */
  /* uint16_t * dibHeaderSize; 
  dataRetrieval((void *) dibHeaderSize, sizeof(uint16_t));
  if(*dibHeaderSize >= 40)
  {
  */
  /* For now, only support BMPINFOHEADER, as CORE doesn't have 16 bit color. */
  dataRetrievalFunc((void *)&loaderState->imageInfo.dibHeader, sizeof(bmp_info_header));
}
