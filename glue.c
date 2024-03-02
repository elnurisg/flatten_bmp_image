#include<stdio.h>
#include<stdlib.h>

int main(int argc, char  *argv[])
{
    int width;
	int height;
	int numOfBitPixel;
	int sizeOfHeader;
	int size;
	unsigned char *header;
	unsigned char *pixels;

    FILE *file = fopen(argv[1], "rw");
    fseek(file, 2, SEEK_SET);
    fread(&size, 1, 4, file);							
	fseek(file, 4, SEEK_CUR);		
	fread(&sizeOfHeader, 1, 4, file);					
	fseek(file, 4, SEEK_CUR);

	fread(&(width), 1, 4, file);						
	fread(&(height), 1, 4, file);					
	fseek(file, 2, SEEK_CUR);	
    fread(&(numOfBitPixel), 1, 2, file);	
	header = (unsigned char *)calloc(sizeOfHeader, sizeof(unsigned char));
	pixels = (unsigned char *)calloc(size - sizeOfHeader, sizeof(unsigned char));

	fseek(file, 0, SEEK_SET);

	fread(header, sizeOfHeader, 1, file);			
	fread(pixels, size - sizeOfHeader, 1, file);

    int byteAmount=numOfBitPixel/8;
    height= -1 * height;
    //-------------------------------------
    FILE *fileNew = fopen("Glued.bmp", "wb");
    
	fwrite(header, sizeOfHeader, 1, fileNew);	 // then we will modify with right size and width 																							
	fseek(fileNew, 3, SEEK_SET);	
    int newWidth =	(argc-1) * width;									// Going to bytes which involve the size
	int newSize = newWidth * height * byteAmount + sizeOfHeader;
	fwrite(&newSize, 4, 1, fileNew);											// Changing headers with new size and new width(means strip size)
	fseek(fileNew, 18, SEEK_SET);										// Going to bytes which involve the width
	fwrite(&newWidth, 4, 1, fileNew);
	fseek(fileNew, sizeOfHeader, SEEK_SET);

	unsigned char *newPixels = (unsigned char *)calloc(newSize - sizeOfHeader, sizeof(unsigned char));

    int widthPixel=width*byteAmount;
    int newWidthPixel=newWidth*byteAmount;
    for(int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "rw");
        fseek(file, sizeOfHeader, SEEK_SET);
        fread(pixels, size - sizeOfHeader, 1, file);

        for(int h = 0; h < height; h++)
        {
            for(int w=0;w<widthPixel;w++){
            newPixels[w + (i-1)*widthPixel + h*newWidthPixel]=pixels[w+h*widthPixel];
            }
        }
    }
            fwrite(&(newPixels[0]), newWidth*height*byteAmount, 1, fileNew);
    return 0;
}