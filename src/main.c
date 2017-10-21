#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <image.h>
#include <bmp.h>

int main(int argc, char *argv[]) {
  // ensure proper usage
  if (argc != 4) {
    fprintf(stderr, "Usage: ./rotate.o infile outfile angle(degrees)\n");
    return 1;
  }

  // remember filenames
  char *infile = argv[1];
  char *outfile = argv[2];
  const double angle = atof(argv[3]);

  if (!angle) {
    fprintf(stderr, "Incorrect angle\n");
    return 2;
  }

  // open input file
  FILE *inptr = fopen(infile, "r");
  if (inptr == NULL) {
    fprintf(stderr, "Could not open %s.\n", infile);
    return 3;
  }

  // open output file
  FILE *outptr = fopen(outfile, "w");
  if (outptr == NULL) {
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile);
    return 4;
  }

  image* src = from_bmp(inptr);
  image* rotated = img_rotate(angle, src);
  img_free(src);

  if (rotated == NULL) {
    fprintf(stderr, "I couldn't rotate your image.\n");
    return 5;
  }

  to_bmp(outptr, rotated);
  img_free(rotated);

  // close infile
  fclose(inptr);

  // close outfile
  fclose(outptr);

  return 0;
}
