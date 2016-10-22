/**
 * Andriy Doroshenko based on Konstantin Stefanenko original idea
 * CS 50
 * pset4    
 * whodunit.c
 * 
 */

       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

typedef struct {
    BYTE original_low;
    BYTE original_high;
    BYTE result_low;
    BYTE result_high;
} linear_range_t;

linear_range_t intensityMap[] = {
    { .original_low =   0, .original_high =   0, .result_low =   0, .result_high =   0 },
    { .original_low =   0, .original_high =  50, .result_low =   0, .result_high =  15 },
    { .original_low =  50, .original_high = 100, .result_low =  15, .result_high =  50 },
    { .original_low = 100, .original_high = 150, .result_low =  50, .result_high =  60 },
    { .original_low = 150, .original_high = 175, .result_low =  60, .result_high = 125 },
    { .original_low = 175, .original_high = 200, .result_low = 125, .result_high = 225 },
    { .original_low = 200, .original_high = 225, .result_low = 225, .result_high = 255 },
    { .original_low = 225, .original_high = 255, .result_low = 255, .result_high = 255 },
    { .original_low =   1, .original_high =   0, .result_low =   0, .result_high =   0 } // stop token, original_low > original_high
};

BYTE linearTransfer(BYTE intencity, linear_range_t *f)
{
    if (f->original_high == f->original_low) {
        return f->result_high;
    }
    return round(((float)(intencity - f->original_low)) / ((float)(f->original_high - f->original_low)) * (f->result_high - f->result_low) + f->result_low);
}

BYTE transfer(BYTE intencity, linear_range_t *map)
{
    for (int i = 0; map[i].original_low <= map[i].original_high; i++) {
        if (map[i].original_low < intencity && intencity <= map[i].original_high) {
            return linearTransfer(intencity, &(map[i]));
        }
    }
    return 0; // found nothing
}

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            BYTE intensity = transfer(triple.rgbtRed, intensityMap);
            triple.rgbtRed = intensity;
            triple.rgbtBlue = intensity;
            triple.rgbtGreen = intensity;
            
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
    }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    //}

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}