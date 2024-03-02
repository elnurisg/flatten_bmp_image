#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "extractStrip.h"


BITMAPINFOHEADER* info_file(FILE *img,BITMAPINFOHEADER *bih){	
	bih = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));

	//nformation about old_picture

	fread(&bih->name[0],1,1,img);
	fread(&bih->name[1],1,1,img);
	fseek(img,2,SEEK_SET);
	fread(&bih->size,4,1,img);
	fseek(img,10,SEEK_SET);
	fread(&bih->start,4,1,img);
	fseek(img,18,SEEK_SET);
	fread(&bih->width,4,1,img);
	fseek(img,22,SEEK_SET);
	fread(&bih->height,4,1,img);
	fseek(img,28,SEEK_SET);
	fread(&bih->bitcount,2,1,img);
	return bih;
}
	

void copy_file(FILE *img,BITMAPINFOHEADER *bih,int n,char **path){
	FILE *new;

	//making correct path_name for file .For example: filename_S100.bmp
	char file_name[25];
	char divide[25];
	int length=strlen(path[0]); 
	for(int i=0;i<length-4;i++)
		divide[i]=path[0][i];

	divide[length-4]='\0';
	sprintf(file_name,"%s_S%d.bmp",divide,n);
	
	new=fopen(file_name,"wb");
	bih=info_file(img,bih);
	int bytes_per_pixel = bih->bitcount/8;
	int old_width=bih->width;

	bih->height *= -1;
	
	bih->header = (unsigned char*)malloc(bih->start*sizeof(unsigned char));//bih->start = 138 in pot images , in example it is 70
	bih->width = n; //n pixels
	bih->size = bytes_per_pixel*(bih->width*bih->height)+bih->start; // size = bytes_per_pixel*(witdh * height) + header_size
	bih->image_size = bih->size - bih->start; //bytes for image = bytes for file - bytes for header
	
	

////// manipulating header in a new file
	fseek(img,0,SEEK_SET);
	fwrite(&bih->name,2,1,new);
	fwrite(&bih->size,4,1,new);		
	fseek(img,6,SEEK_SET);

	fread(bih->header,12,1,img);
	fwrite(bih->header,12,1,new);		
	
	fseek(img,18,SEEK_SET);
	fwrite(&bih->width,4,1,new);
	fseek(img,22,SEEK_SET);

	fread(bih->header,bih->start - 22,1,img);
	fwrite(bih->header,bih->start - 22,1,new);		
	

	free(bih->header);

	//dealing with interval.... if user wants 100 pixel it will cut [-50:+50] from middle
	int strip_start=old_width/2 - bih->width/2;
	bih->pixels = (unsigned char*)calloc(old_width * bih->height * bytes_per_pixel, sizeof(unsigned char));
	fread(bih->pixels,1,old_width*bih->height*bytes_per_pixel,img);

	//determine how many bytes do we need for each row
	int pad = (4-(bih->width*bytes_per_pixel)%4)%4;
	
	fseek(new,bih->start,SEEK_SET);
	
	int color=0;
	for(int i=0;i<bih->height;i++){
		fwrite(&(bih->pixels[(i*old_width+strip_start)*bytes_per_pixel]),1,bih->width*bytes_per_pixel,new);
		if(pad != 0)
			fwrite(&color,1,pad,new);
	}


	free(bih->pixels);
	fclose(img);
	fclose(new);
}

/*this function will copy string to another except 1 st index....
 so if user enters -100 , function will return 100
*/
void deal_argument(char **src,char *dest){
	int j=0;
	for(int i=1; i < strlen(src[1]) ; i++){
		dest[j]=src[1][i];
		j++;
	}
		dest[j]='\0';
}


/* this function checks if user gives */

int check_bmp(FILE *img){
	BITMAPINFOHEADER as;
	fread(as.name,2,1,img);
	fseek(img,0,SEEK_SET);
	char test[3]={'B','M','\0'};
	//if(strcmp(test,as.name) != 0){
//printf("ayridi\n");
		
	return !strcmp(test,as.name);
	//return 0;
}










