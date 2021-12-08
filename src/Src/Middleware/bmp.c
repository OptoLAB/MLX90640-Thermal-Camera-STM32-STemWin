#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"
#include "ff.h"
#include "img.h"

#define IMG_SIZE	(IMG_X_SIZE*IMG_Y_SIZE*3)
#define HDR_SIZE	(54)
#define INFO_SIZE	(54-14)
int img_cnt = 0;
uint8_t image_buf[IMG_X_SIZE*3] = {0};   //image is recored line by line
static uint8_t bmp_header[HDR_SIZE]={0};

// index	info
//	0		'BM'
//	2		file size = x_size*y_size*3+header_size(54)
//	6  		reserved (unused)
//	10		data ofset = header_size
//	14  	size of info header = header_size-14
//	18  	width
//	22  	height
//	26 		num of planes = 1
//	28   	bpp = 24
//	30  	compresion = 0 (no compresion)
//	34  	img size = x_size*y_size*3
//	38  	horizontal res
//	42  	vertical res
//	46  	colors used
//	50		color important

void prepareBMPheader(uint8_t *header)
{
	uint8_t i;
	for(i=0;i<HDR_SIZE;i++)header[i]=0x00;
	header[0]=0x42;
	header[1]=0x4D;
	header[2]=((IMG_SIZE+HDR_SIZE))&0xFF;
	header[3]=((IMG_SIZE+HDR_SIZE)>>8)&0xFF;
	header[4]=((IMG_SIZE+HDR_SIZE)>>16)&0xFF;
	header[5]=((IMG_SIZE+HDR_SIZE)>>24)&0xFF;
	header[10]=HDR_SIZE;
	header[14]=INFO_SIZE;
	header[18]=((IMG_X_SIZE))&0xFF;
	header[19]=((IMG_X_SIZE)>>8)&0xFF;
	header[22]=((IMG_Y_SIZE))&0xFF;
	header[23]=((IMG_Y_SIZE)>>8)&0xFF;
	header[26]=1;	//planes
	header[28]=24;	//bpp
	header[34]=((IMG_SIZE))&0xFF;
	header[35]=((IMG_SIZE)>>8)&0xFF;
	header[36]=((IMG_SIZE)>>16)&0xFF;
	header[37]=((IMG_SIZE)>>24)&0xFF;
}

void captureBMPImage(void)
{

  uint16_t  i, j;
  uint32_t  data = 0;
  UINT bw = 0;
  uint8_t R,G,B;
  uint8_t pixel_size;
  char file_str[10] = {0};
  FIL file;

  if(getInterpolate())pixel_size=(uint8_t)PX_SIZE/INT_POINTS;
  else pixel_size=PX_SIZE;

  prepareBMPheader(bmp_header);
  sprintf(file_str, "img%d.bmp",img_cnt);
  f_open(&file, file_str, FA_WRITE | FA_CREATE_ALWAYS);

  //write header
  f_write(&file, bmp_header, 54, &bw);

  //write pixel data
  uint8_t x=0,y=0;

  for (j = 0; j < IMG_Y_SIZE; j++)
  {
	y=(uint8_t)(((IMG_Y_SIZE-1)-j)/pixel_size);
	uint16_t k=0;
    for(i=0;i<IMG_X_SIZE;i++)
    {
      x=(uint8_t)(i/pixel_size);
      data = getPixelColor(getImagePixel(x,y));
      R = ((data>>16)&0xFF);
      G = ((data>>8)&0xFF);
      B = ((data)&0xFF);
      image_buf[k+2] = B;
      image_buf[k+1] = G;
      image_buf[k+0] = R;
      k+=3;
    }
    f_write(&file, image_buf, IMG_X_SIZE*3, &bw);
  }
  //PADDING NOT NEEDED!!!

  f_close(&file);
  img_cnt++;
}



