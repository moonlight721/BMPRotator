#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool rotate(const double angle, FILE * const in, FILE* const out);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./rotate infile outfile angle(degrees)\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];
    const double angle = atof(argv[3]);

    if (!angle)
    {
        fprintf(stderr, "Incorrect angle\n");
        return 2;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    puts("I've went to rotate");
    bool result = rotate(angle, inptr, outptr);
    puts("I've returned from rotate");

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    if (result) return 0;
    else 
    {
        fprintf(stderr, "I couldn't rotate your image.\n");
        return 5;
    }
}

