#include<stdio.h>

typedef struct {
   unsigned char name[2];
   unsigned int width;
   unsigned int height;
   unsigned int header_size;//138 byte
   unsigned int start;//at which byte image starts 
   unsigned short bitcount; // shows how many bytes for 1 pixels
   unsigned int size; //size of the file
   unsigned int image_size;
   unsigned char *pixels;
   unsigned char *header; 
   
   
} BITMAPINFOHEADER;



BITMAPINFOHEADER* info_file(FILE *img,BITMAPINFOHEADER *bih);

void read_file(FILE *img,BITMAPINFOHEADER *bih,int n);
void copy_file(FILE *img,BITMAPINFOHEADER *bih,int n,char **src);
void deal_argument(char **src,char *dest);
int check_bmp(FILE *img);



