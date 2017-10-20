/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <bmp.h>

#define RIGHT true
#define LEFT false

bool rotate(const bool side, FILE * const in, FILE* out);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./rotate infile outfile side\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];
    bool side = LEFT;

    if (strcmp("RIGHT", argv[3]) || strcmp("right", argv[3]) 
        || strcmp("1", argv[3]) || strcmp("true", argv[3]))
        side = RIGHT;

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    bool result = rotate(side, inptr, outptr);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    if (result) return 0;
    else 
    {
        fprintf(stderr, "I couldn't rotate your image.\n");
        return 4;
    }
}

bool rotate(const bool side, FILE * const in, FILE* out)
{
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

    // swapping width and height because of rotating
    bi.biWidth+=bi.biHeight; // bi.biWidth contains width + height
    bi.biHeight = bi.biWidth - bi.biHeight; // bi.biHeight contans width
    bi.biWidth = bi.biWidth - bi.biHeight; // bi.biWidth contans height

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // height and width are swapped
    RGBTRIPLE inp[bi.biWidth][bi.biHeight];

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biWidth); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0, biWidth = abs(bi.biHeight); j < biWidth; j++)
            // read RGB triple from infile
            fread(&inp[i][j], sizeof(RGBTRIPLE), 1, in);

        // skip over padding, if any
        fseek(in, padding, SEEK_CUR);
    }

     
    // iterate over outfile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in line
        for (int j = 0, biWidth = abs(bi.biWidth); j < biWidth; j++)
            // write RGB triple to outfile
            fwrite(&inp[j][i], sizeof(RGBTRIPLE), 1, out);

        // add padding
        for (int k = 0; k < padding; k++)
            fputc(0x00, out);
    }

    return false;
}

