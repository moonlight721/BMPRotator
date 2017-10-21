#pragma once
#include <stdint.h>

#ifndef BYTE
typedef uint8_t BYTE;
#endif

typedef struct pixel {
  BYTE red;
  BYTE green;
  BYTE blue;
} pixel;

typedef struct image {
  int height;
  int width;
  pixel** pixels;
} image;

image* img_rotate(const double, image* const);
void img_free(image*);
