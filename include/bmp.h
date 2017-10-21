#include <stdint.h>
#include <stdbool.h>
#include <image.h>
#include <stdio.h>

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

typedef struct {
  WORD type;
  DWORD size;
  WORD reserved1;
  WORD reserved2;
  DWORD off_bits;
} __attribute__((__packed__))  
BITMAPFILEHEADER;

typedef struct {
  DWORD size;
  LONG width;
  LONG height;
  WORD planes;
  WORD bit_count;
  DWORD compression;
  DWORD size_image;
  LONG x_pels_per_meter;
  LONG y_pels_per_meter;
  DWORD clr_used;
  DWORD clr_important;
} __attribute__((__packed__))  
BITMAPINFOHEADER;

image* from_bmp(FILE* const);
void to_bmp(FILE* const, image* const);
