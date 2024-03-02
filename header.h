#include<stdio.h>
typedef struct image
{
	int width;
	int height;
	int numColor;
	int numOfBitPixel;
	int sizeOfHeader;
	int size;
	unsigned char *header;
	unsigned char *pixels;
	FILE *file;
} im;
float maximum(float array[12]);
void store(float array[12]); // find maximum and store psf
void readFile(im imgSource, char* fileName);
void writeFile(im imgTarget, im img, char* fileName);