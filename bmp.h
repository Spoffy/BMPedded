/**
  Library for reading BMP images and displaying them on LaFortuna
**/
#include <stdint.h>
#include <string.h>
#include "lcd.h"

typedef enum {READ=0,SKIP=1} data_read_type_t;

typedef struct {
  void * buffer;
  size_t bufferSize;
  size_t dataOffset;
} bmp_data_request;

typedef void (*bmp_need_more_bytes)(bmp_data_request * request);

typedef enum {STATUS_OK=0} status_t; 

typedef struct __attribute__ ((packed)) {
  uint16_t fileIdentifier;
  uint32_t fileSize;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t imageDataOffset;
} bmp_file_header;

typedef struct __attribute__ ((packed)) {
  uint32_t headerSize;
  uint16_t imageWidth;
  uint16_t imageHeight;
  uint16_t colorPlanes;
  uint16_t bitsPerPixel;
} bmp_core_header;

typedef struct __attribute__ ((packed)) {
  uint32_t headerSize;
  int32_t imageWidth;
  int32_t imageHeight;
  uint16_t colorPlanes;
  uint16_t bitsPerPixel;
  uint32_t compressionMethod;
  uint32_t imageSize; /*Size of raw bitmap data*/
  int32_t horizontalRes;
  int32_t verticalRes;
  uint32_t colorsInPalette;
  uint32_t numImportantColors;
} bmp_info_header;

typedef struct {
  bmp_need_more_bytes data_request_func;
  size_t rowSize;
  uint16_t currentRow;
  uint16_t * imageDataRow;
  bmp_file_header fileHeader;
  bmp_info_header dibHeader;
} bmp_image_loader_state;


status_t init_bmp(bmp_image_loader_state * loaderState, bmp_need_more_bytes dataRetrievalFunc);

status_t bmp_next_row(bmp_image_loader_state * loaderState);

/* Move this out to its own file, so that lcd.h isn't necessary for library usage? */
status_t fill_rectangle_bmp(rectangle * area, const bmp_image_loader_state * loaderState);
