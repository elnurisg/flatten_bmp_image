#include<stdio.h>
#include<stdlib.h>	
#include <string.h>
#include "header.h"

float psf[12];

void store(float array[12]){
    float max=maximum(array);
    for(int i=0;i<12;i++) {
        psf[i]=max/array[i];
    }
}

float maximum(float array[12]){
    float max=array[0];
    for(int i=0;i<12;i++) {
        if (array[i]>max){
            max=array[i];
        }}
    return max;
}


void readFile(im imgSource, char* fileName)
{
	char *bm = (char *)calloc(2, sizeof(char));
	fread(bm, 2, 1, imgSource.file);
	if (!(bm[0] == 'B' && bm[1] == 'M'))										// Checking first two byte are BM or no 
	{
		fprintf(stderr, "\nError:\n\tWrong file type. First two bytes are not BM\n");
		exit(0);
	}
	fread(&imgSource.size, 1, 4, imgSource.file);							// reading size of original bmp file
	fseek(imgSource.file, 4, SEEK_CUR);		
	fread(&imgSource.sizeOfHeader, 1, 4, imgSource.file);					// reading size of headers of original bmp file
	fseek(imgSource.file, 4, SEEK_CUR);

	fread(&(imgSource.width), 1, 4, imgSource.file);						// reading width of original bmp file
	fread(&(imgSource.height), 1, 4, imgSource.file);						// reading height of original bmp file
	fread(&(imgSource.numColor), 1, 2, imgSource.file);						// reading number of color planes of original bmp file
	fread(&(imgSource.numOfBitPixel), 1, 2, imgSource.file);					// reading number of bits per pixels of original bmp file
	imgSource.header = (unsigned char *)calloc(imgSource.sizeOfHeader, sizeof(unsigned char));
	imgSource.pixels = (unsigned char *)calloc(imgSource.size - imgSource.sizeOfHeader, sizeof(unsigned char));

	fseek(imgSource.file, 0, SEEK_SET);

	fread(imgSource.header, imgSource.sizeOfHeader, 1, imgSource.file);				// reading header bytes of original bmp file
	fread(imgSource.pixels, imgSource.size - imgSource.sizeOfHeader, 1, imgSource.file);				// reading pixel bytes of original bmp file

    im imgTarget;
    writeFile(imgTarget, imgSource, fileName);
}

void writeFile(im imgTarget, im img, char* fileName)
{	
    float psfMaximal=maximum(psf);
    int width=480*psfMaximal;
    int height=2230;

	//copy the modified pixels ....width,size will be changed, and pixels will be changed too because of cutting photo
	int byteAmount=img.numOfBitPixel/8;

	int length=strlen(fileName);		// Length of original_name.bmp
	sprintf(&fileName[length-4],"_Resized.bmp");       // Changing name like  original_name_Resized.bmp where
    imgTarget.file = fopen(fileName, "wb");
    
	fwrite(img.header, img.sizeOfHeader, 1, imgTarget.file);	 // then we will modify with right size and width 																							
	fseek(imgTarget.file, 3, SEEK_SET);											// Going to bytes which involve the size
	int size = width * height *byteAmount + img.sizeOfHeader;
	fwrite(&size, 4, 1, imgTarget.file);											// Changing headers with new size and new width(means strip size)
	fseek(imgTarget.file, 18, SEEK_SET);										// Going to bytes which involve the width
	fwrite(&width, 4, 1, imgTarget.file);
	fseek(imgTarget.file, img.sizeOfHeader, SEEK_SET);
	
		imgTarget.pixels = (unsigned char *)calloc(size - img.sizeOfHeader, sizeof(unsigned char));
	
		float maxCoef=psf[0]; float coefGlob=maxCoef;  int i=0; float coef=0; float remainder=0;
		float curPsf=psf[0];  int blackShift=0;
		for(int h=0;h<2230;h++){

		int x=(int)(480*curPsf*byteAmount)%4,y=0;
			
			if(h%200==0){ maxCoef=psf[h/200];curPsf=psf[h/200];coefGlob=maxCoef;}
		
			i=480*byteAmount*h;

			int j=width*byteAmount*h;
			for(j=width*byteAmount*h;j<(width*byteAmount*(h+1)-480*(psfMaximal-curPsf)*byteAmount/2) ;j+=4){   // index of fTarget
				blackShift=480*(psfMaximal-curPsf)*byteAmount/2;
				while(blackShift%byteAmount!=0) blackShift++;
				if(j==width*byteAmount*h)	j+=blackShift;   //for first bit

				if(coefGlob>=1){coef=1;}
				else { coef=coefGlob; remainder=1-coef;}

					imgTarget.pixels[j]=coef*img.pixels[i] + (1-coef)*img.pixels[i+4];
					imgTarget.pixels[j+1]=coef*img.pixels[i+1] + (1-coef)*img.pixels[i+4+1];
					imgTarget.pixels[j+2]=coef*img.pixels[i+2] + (1-coef)*img.pixels[i+4+2];
					imgTarget.pixels[j+3]=img.pixels[i+3];
				 
					coefGlob-=coef;
					if(coefGlob<=0){ coefGlob=maxCoef;i+=4;}
					coefGlob-=remainder;
					remainder=0;
			}
				if(x!=0){for(int p=0;p<4-x;p++) imgTarget.pixels[j+p]=y;} //padding
		}

	fwrite(&(imgTarget.pixels[0]), byteAmount*width*2230, 1, imgTarget.file);

}


