#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <bmp.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

bool rotate(double angle, FILE *const in, FILE *const out) {
  // read infile's BITMAPFILEHEADER
  BITMAPFILEHEADER bf;
  fread(&bf, sizeof(BITMAPFILEHEADER), 1, in);

  // read infile's BITMAPINFOHEADER
  BITMAPINFOHEADER bi;
  fread(&bi, sizeof(BITMAPINFOHEADER), 1, in);

  // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
      bi.biBitCount != 24 || bi.biCompression != 0)
    return false;

  // write outfile's BITMAPFILEHEADER
  fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);

  // store values
  const LONG src_height = bi.biHeight;
  const LONG src_width = bi.biWidth;

  // determine padding for scanlines
  int padding = (4 - (src_width * sizeof(RGBTRIPLE)) % 4) % 4;

  RGBTRIPLE inp[src_height][src_width];

  // iterate over infile's scanlines
  for (int i = 0; i < src_height; i++) {
    // iterate over pixels in scanline
    for (int j = 0; j < src_width; j++) {
      // read RGB triple from infile
      fread(&inp[i][j], sizeof(RGBTRIPLE), 1, in);
    }

    // skip over padding, if any
    fseek(in, padding, SEEK_CUR);
  }

  angle *= 2 * M_PI / 360;  // radians
  const double cosine = cos(angle);
  const double sine = sin(angle);

  // some points
  const double point1x = -src_height * sine;
  const double point1y = src_height * cosine;
  const double point2x = src_width * cosine - src_height * sine;
  const double point2y = src_height * cosine + src_width * sine;
  const double point3x = src_width * cosine;
  const double point3y = src_width * sine;

  const double minx = min(0, min(point1x, min(point2x, point3x)));
  const double miny = min(0, min(point1y, min(point2y, point3y)));
  const double maxx = max(0, max(point1x, max(point2x, point3x)));
  const double maxy = max(0, max(point1y, max(point2y, point3y)));

  bi.biWidth = (int)ceil(maxx - minx);
  bi.biHeight = (int)ceil(maxy - miny);

  padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

  // write outfile's BITMAPINFOHEADER
  fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

  RGBTRIPLE ou[bi.biHeight][bi.biWidth];

  for (int x = 0; x < bi.biWidth; x++) {
    for (int y = 0; y < bi.biHeight; y++) {
      int srcx = (int)((x + minx) * cosine + (y + miny) * sine);
      int srcy = (int)((y + miny) * cosine - (x + minx) * sine);
      if (srcx >= 0 && srcx < src_width && srcy >= 0 && srcy < src_height) {
        ou[y][x] = inp[srcy][srcx];
      }
    }
  }

  for (int i = 0; i < bi.biHeight; i++) {
    for (int j = 0; j < bi.biWidth; j++) {
      fwrite(&ou[i][j], sizeof(RGBTRIPLE), 1, out);
    }
    // add padding
    for (int k = 0; k < padding; k++) fputc(0x00, out);
  }

  return true;
}
