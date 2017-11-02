#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <image.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

image* img_alloc(const int height, const int width) {
  image* img = malloc(sizeof(image));
  img->height = height;
  img->width = width;
  img->pixels = calloc(height, sizeof(pixel*));
  for (int i = 0; i < height; i++) {
    img->pixels[i] = calloc(width, sizeof(pixel));
  }
  return img;
}

image* img_rotate(const double degrees, image* const src_img) {
  const double angle = 2 * M_PI * degrees / 360;  // radians
  const double cosine = cos(angle);
  const double sine = sin(angle);

  const int src_height = src_img->height;
  const int src_width = src_img->width;

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

  const int height = (int)ceil(fabs(maxy) - miny);
  const int width = (int)ceil(fabs(maxx) - minx);

  image * rotated = img_alloc(height, width);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int srcx = (int)ceil((x + minx) * cosine + (y + miny) * sine);
      int srcy = (int)ceil((y + miny) * cosine - (x + minx) * sine);
      if (srcx >= 0 && srcx < src_width && srcy >= 0 &&
          srcy < src_height) {
        rotated->pixels[y][x] = src_img->pixels[srcy][srcx];
      }
    }
  }

  return rotated;
}

image* img_blur (const image * const src, const double r) {
    const int rs = (int) ceil(r * M_E);     // significant radius
    const int h = src -> height;
    const int w = src -> width;
    image * const target = img_alloc(h,w);
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++) {
            long double red_val = 0, red_wsum = 0,
              green_val = 0, green_wsum = 0,
              blue_val = 0, blue_wsum = 0;
            for(int iy = i-rs; iy < i + rs + 1; iy++)
                for(int ix = j-rs; ix < j + rs + 1; ix++) {
                    const int x = min(w-1, max(0, ix));
                    const int y = min(h-1, max(0, iy));
                    const long double dsq = (ix - j) * (ix - j) + (iy - i) * (iy - i);
                    const long double wght = exp( -dsq / (2 * r * r) ) / (M_PI * 2 * r * r);
                    red_val += (src -> pixels[y][x].red) * wght;  
                    green_val += (src -> pixels[y][x].green) * wght;  
                    blue_val += (src -> pixels[y][x].blue) * wght;  
                    red_wsum += wght;
                    green_wsum += wght;
                    blue_wsum += wght;
                }
            target->pixels[i][j].red = round(red_val/red_wsum);            
            target->pixels[i][j].green = round(green_val/green_wsum);            
            target->pixels[i][j].blue = round(blue_val/blue_wsum);            
        }
        return target;
}

void img_free(image* img) {
  for (int i = 0; i < img->height; i++) free(img->pixels[i]);
  free(img->pixels);
  free(img);
}
