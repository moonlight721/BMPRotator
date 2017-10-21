#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <image.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

image* img_rotate(const double degrees, image* const src_img) {
  const double angle = 2 * M_PI * degrees / 360;  // radians
  const double cosine = cos(angle);
  const double sine = sin(angle);

  const double point1x = -src_img->height * sine;
  const double point1y = src_img->height * cosine;
  const double point2x = src_img->width * cosine - src_img->height * sine;
  const double point2y = src_img->height * cosine + src_img->width * sine;
  const double point3x = src_img->width * cosine;
  const double point3y = src_img->width * sine;

  const double minx = min(0, min(point1x, min(point2x, point3x)));
  const double miny = min(0, min(point1y, min(point2y, point3y)));
  const double maxx = max(0, max(point1x, max(point2x, point3x)));
  const double maxy = max(0, max(point1y, max(point2y, point3y)));

  image* rotated = malloc(sizeof(image));
  rotated->height = (int)ceil(fabs(maxy) - miny);
  rotated->width = (int)ceil(fabs(maxx) - minx);

  rotated->pixels = calloc(rotated->height, sizeof(pixel*));

  for (int i = 0; i < rotated->height; i++) {
    rotated->pixels[i] = calloc(rotated->width, sizeof(pixel));
  }

  for (int x = 0; x < rotated->width; x++) {
    for (int y = 0; y < rotated->height; y++) {
      int srcx = (int)ceil((x + minx) * cosine + (y + miny) * sine);
      int srcy = (int)ceil((y + miny) * cosine - (x + minx) * sine);
      if (srcx >= 0 && srcx < src_img->width && srcy >= 0 && srcy < src_img->height) {
        rotated->pixels[y][x] = src_img->pixels[srcy][srcx];
      }
    }
  }

  return rotated;
}

void img_free(image* img) {
  for (int i = 0; i < img->height; i++) free(img->pixels[i]);
  free(img);
}
