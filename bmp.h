/**
  Library for reading BMP images and displaying them on LaFortuna
**/
#include "lcd.h"

typedef void (*bmp_need_more_bytes)(void * byteBuffer, size_t bufferSize);

/* Structs need packing, as we load directly into them. */
typedef struct {
  bmp_need_more_bytes data_request_func;
  uint16_t * imageDataRow;
  bmp_image_info imageInfo;
} bmp_image_loader_state;

typedef struct {
  uint16_t fileIdentifier;
  uint32_t fileSize;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t imageDataOffset;
} bmp_file_header;

typedef struct {
  uint32_t headerSize;
  uint16_t imageWidth;
  uint16_t imageHeight;
  uint16_t colorPlanes;
  uint16_t bitsPerPixel;
} bmp_core_header;

typedef struct {
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
  bmp_file_header fileHeader;
  bmp_info_header dibHeader;
} bmp_image_info;

uint8_t init_bmp(bmp_image_loader_state * loaderState, bmp_need_more_bytes dataRetrievalFunc);

uint8_t bmp_next_row(const bmp_image_loader_state * loaderState);

uint8_t fill_rectangle_bmp(rectangle * area, const bmp_image_loader_state * loaderState);
