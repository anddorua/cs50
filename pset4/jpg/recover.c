/**
 * Andriy Doroshenko
 * mapleukraine@gmail.com
 * 
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>

#define SRC_FILE_NAME "card.raw"
#define BLOCK_SIZE 512

typedef struct {
    FILE* outptr;
    int index;
} out_file_info;

void closeOutStream(out_file_info *ofi)
{
    if (ofi->outptr) {
        fclose(ofi->outptr);
        ofi->outptr = NULL;
    }
}

int beginNewFile(out_file_info *ofi)
{
    if (ofi->outptr) {
        closeOutStream(ofi);
    }
    char outfilename[13];
    sprintf(outfilename, "%.3d.jpg", (ofi->index)++);
    if (!(ofi->outptr = fopen(outfilename, "w")))
    {
        fprintf(stderr, "Could not create %s.\n", outfilename);
        return 0;
    } else {
        return 1;
    }
}

int saveBlock(unsigned char *buf, FILE* outfile)
{
    if (!fwrite(buf, BLOCK_SIZE, 1, outfile)) {
        fprintf(stderr, "Could not write to out file.\n");
        return 0;
    } else {
        return 1;
    };
}

int isStartingBlock(unsigned char *buf)
{
    return buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff
        && ((buf[3] & 0xe0) == 0xe0);
}

int collectBlock(unsigned char *buf, out_file_info *ofi)
{
    if (isStartingBlock(buf)) {
        if (!beginNewFile(ofi)){
            return 0;
        }
    }
    if (ofi->outptr) {
        return saveBlock(buf, ofi->outptr);
    } else {
        return 1;
    }
}

int main(int argc, char* argv[])
{
    unsigned char buf[BLOCK_SIZE];
    out_file_info outstream = {NULL, 0};
    FILE* inptr = fopen(SRC_FILE_NAME, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", SRC_FILE_NAME);
        return 1;
    }
    
    int blocks_count = 0;
    while (1) {
        fread(buf, BLOCK_SIZE, 1, inptr);
        if (feof(inptr)) {
            break;
        }
        
        blocks_count++;
        if (!collectBlock(buf, &outstream)) {
            break;
        }
    }

    closeOutStream(&outstream);

    // close infile
    fclose(inptr);

}
