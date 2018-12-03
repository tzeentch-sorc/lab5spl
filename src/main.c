#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <malloc.h>


struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;

    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
struct pixel{
    unsigned char b, g, r;
};
int main(int argc, char **argv) {
    char *fn = "./../res/custodes1.bmp";
    FILE *f = fopen(fn, "rb");
    FILE *r = fopen("rot.bmp", "wb");

    struct bmp_header header;
    fread(&header, sizeof(struct bmp_header), 1, f);
    int start = ftell(f); //пропуск до пикселей, custodes = 0
    fseek(f, header.bOffBits, SEEK_SET);
    int end = ftell(f);


    struct bmp_header header_new = header;//setting new header
    header_new.biWidth = header.biHeight;
    header_new.biHeight = header.biWidth;

    fwrite(&header_new, sizeof(struct bmp_header), 1, r); //writing header to new file
    fseek(r, end-start, SEEK_CUR); //setting the cursor on pixels

    struct pixel xy;
//    int cur = 0;
    for (long x = 0; x < header.biWidth; x++)  {
        for(long y = header.biHeight - 1; y >= 0; y--) {
            fseek(f, y * header.biWidth * sizeof(struct pixel) + x * sizeof(struct pixel) + header.bOffBits, SEEK_SET);
//            cur = ftell(f);
//            printf("%d\n", cur);
            fread(&xy, sizeof(struct pixel), 1, f);
            fwrite(&xy, sizeof(struct pixel), 1, r);
        }
    }

    fclose(r);
    return 0;
}