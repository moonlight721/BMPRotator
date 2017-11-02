#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <bmp.h>
#include <image.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, "Usage: ./rotate.o infile outfile angle(degrees) radius(for blur)\n");
    return 1;
  }

  char *infile = argv[1];
  char *outfile = argv[2];
  const double angle = atof(argv[3]);

  if (!angle) {
    fprintf(stderr, "Incorrect angle\n");
    return 2;
  }

  const double radius = atof(argv[4]);

  if (radius <= 0) {
    fprintf(stderr, "Incorrect radius\n");
    return 4;
  }

  FILE *inptr = fopen(infile, "r");
  if (inptr == NULL) {
    fprintf(stderr, "Could not open %s.\n", infile);
    return 4;
  }

  FILE *outptr = fopen(outfile, "w");
  if (outptr == NULL) {
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile);
    return 5;
  }

  image *src = from_bmp(inptr);
  if (!src) {
    fprintf(stderr, "Could not recognize %s\n", infile);
    fclose(inptr);
    fclose(outptr);
    return 6;
  }
  image *rotated = img_rotate(angle, src);
  img_free(src);
  image *blured = img_blur(rotated, radius);
  img_free(rotated);

  to_bmp(outptr, blured);
  img_free(blured);

  fclose(inptr);
  fclose(outptr);

  return 0;
}
