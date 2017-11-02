#include <bmp.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>

image* from_bmp(FILE* inptr) {
  BITMAPFILEHEADER bf;
  fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

  BITMAPINFOHEADER bi;
  fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

  if (bf.type != 0x4d42 || bf.off_bits != 54 || bi.size != 40 ||
      bi.bit_count != 24 || bi.compression != 0) {
    return NULL;
  }

  image* img = malloc(sizeof(image));
  img->height = bi.height;
  img->width = bi.width;
  img->pixels = calloc(img->height, sizeof(pixel*));

  int padding = (4 - (img->width * sizeof(pixel)) % 4) % 4;

  for (int i = 0; i < img->height; i++) {
    img->pixels[i] = calloc(img->width, sizeof(pixel));
    for (int j = 0; j < img->width; j++) {
      fread(&img->pixels[i][j], sizeof(pixel), 1, inptr);
    }

    fseek(inptr, padding, SEEK_CUR);
  }
  return img;
}

void to_bmp(FILE* outptr, image* img) {
  BITMAPINFOHEADER bi;

  bi.bit_count = 24;
  bi.compression = 0;
  bi.size = 40;
  bi.planes = 1;
  bi.height = img->height;
  bi.width = img->width;
  bi.size_image = bi.height * bi.width * sizeof(pixel);
  bi.x_pels_per_meter = 0;
  bi.y_pels_per_meter = 0;
  bi.clr_used = 0;
  bi.clr_important = 0;

  BITMAPFILEHEADER bf;
  bf.type = 0x4d42;
  bf.off_bits = 54;
  bf.size = bi.size + bf.off_bits;
  bf.reserved1 = 0;
  bf.reserved2 = 0;

  fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
  fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

  int padding = (4 - (img->width * sizeof(pixel)) % 4) % 4;

  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      fwrite(&img->pixels[i][j], sizeof(pixel), 1, outptr);
    }
    for (int k = 0; k < padding; k++) fputc(0x00, outptr);
  }
}
