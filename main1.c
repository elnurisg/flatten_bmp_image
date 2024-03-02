#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "extractStrip.h"

int main(int argc , char **argv){

	if(argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))){
		printf("Usage : %s -pixel filename.bmp\n",argv[0]);
		return 0;
	}

	if(argc < 3){
		printf("ERROR:use\n %s -h or %s --help\n",argv[0],argv[0]); 
		return 0;
	}

	char dest[50];
	deal_argument(argv,dest);
	
	if(argv[1][0] != '-'){
		printf("ERROR:use\n %s -h or %s --help\n",argv[0],argv[0]); 
		return 0;
	}

	FILE *image;
	BITMAPINFOHEADER bih;
	image=fopen(argv[2],"rb+");

	if(image == NULL){
		fprintf(stderr,"couldnt open the file !!!\nUsage : ./extractStrip -pixel filename.bmp");
		return 0;
	}

	// if(check_bmp(image) == 0){
	// 	printf("picture is not .bmp format!!!\n");
	// 	return 0;
	// }

	
   // read_file(image,&bih,n); 
    int x=atoi(dest);// copy char to int 
	copy_file(image,&bih,x,&argv[2]);

	
}
