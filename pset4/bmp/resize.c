/**
 * resize.c
 *
 * Andriy Doroshenko
 * 
 * Computer Science 50
 * Problem Set 4
 *
 * Enlarges a BMP in N times.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int calcPadding(int pixelWidth)
{
    return (4 - (pixelWidth * sizeof(RGBTRIPLE)) % 4) % 4;
}

int calcStructureSize(int width, int height)
{
    return (width * sizeof(RGBTRIPLE) + calcPadding(width)) * height;
}

void createResizedFileHeader(BITMAPFILEHEADER* out, BITMAPFILEHEADER in, BITMAPINFOHEADER* info)
{
    (*out) = in;
    //out->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + info->biSizeImage;
    out->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) 
        + calcStructureSize(info->biWidth, abs(info->biHeight));
}

void createResizedFileInfoHeader(BITMAPINFOHEADER* out, BITMAPINFOHEADER in, int ratio)
{
    (*out) = in;
    out->biWidth = in.biWidth * ratio;
    out->biHeight = in.biHeight * ratio;
    out->biSizeImage = calcStructureSize(out->biWidth, abs(out->biHeight));
}

void writeTripleMultiple(RGBTRIPLE* triple, int ratio, FILE* file)
{
    for (int i = 0; i < ratio; i++)
    {
        fwrite(triple, sizeof(RGBTRIPLE), 1, file);
    }
}

void copyScanLineMultiple(FILE *outptr, FILE *inptr, int srcLinePixelsAmount, int ratio)
{
    fpos_t bookmark;
    RGBTRIPLE triple;
    fgetpos(inptr, &bookmark);
    int outPaddingCount = calcPadding(srcLinePixelsAmount * ratio);
    int inPaddingCount = calcPadding(srcLinePixelsAmount);
    for (int i = 0; i < ratio; i++)
    {
        // reset position to original position of scan line
        fsetpos(inptr, &bookmark);
        // scan each line
        for (int j = 0; j < srcLinePixelsAmount; j++)
        {
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            writeTripleMultiple(&triple, ratio, outptr);
        }
        //finish each line with padding
        for (int k = 0; k < outPaddingCount; k++)
        {
            fputc(0x00, outptr);
        }
    }
    // skip over padding, if any
    fseek(inptr, inPaddingCount, SEEK_CUR);
}

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    int ratio = atoi(argv[1]);
    if (!(0 < ratio && ratio <= 100)) 
    {
        printf("n must be between 1 and 100 inclusuve\n");
        return 5;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

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

    BITMAPFILEHEADER bfOut = bf;
    BITMAPINFOHEADER biOut = bi;
    createResizedFileInfoHeader(&biOut, bi, ratio);
    createResizedFileHeader(&bfOut, bf, &biOut);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bfOut, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biOut, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        copyScanLineMultiple(outptr, inptr, bi.biWidth, ratio);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
