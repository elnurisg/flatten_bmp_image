#include<stdio.h>
#include<stdlib.h>  
#include <string.h>
#include "header.h"

int main(int argc, char  *argv[])
{
    float pixelShift[]={35,40,45,51,58,60,63,66,67,69,70,71};
    store(pixelShift);

    if(argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))){
    printf("Usage : %s filename.bmp\n",argv[0]);
    return 0;
  }

  if(argc < 2){
    printf("ERROR:use following\n %s -h or %s --help\n",argv[0],argv[0]); 
    return 0;
  }

    char *fileName=argv[1];
	char *fileType=(char*)calloc(4,sizeof(char));
  	int length=strlen(argv[1]);                         //Checking fileType means end of file name is .bmp or not
 	for(int i=4;i>0;i--) fileType[4-i]=fileName[length-i];
  	if((strcmp(fileType,".bmp"))){ fprintf(stderr,"\nError:\n\tWrong file type. End of file name is not '.bmp'\n"); return(0);}
    
	im imgSource;
	imgSource.file = fopen(fileName, "rw");
  if(imgSource.file == NULL){
    fprintf(stderr,"Couldn't open the file !!!\nUsage : ./resize filename.bmp\n");
    return 0;
  }
    readFile(imgSource, fileName);			

    return 0;
}