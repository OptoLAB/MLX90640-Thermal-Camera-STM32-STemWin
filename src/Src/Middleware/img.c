#include "img.h"
#include "pallettes.h"

//defaults
float minTemp=20.0f, maxTemp=37.0f;
uint8_t mode=0;
uint8_t interpolate=1, autoscale=1;

//images
uint32_t thermal_image[PX_Y][PX_X];
uint32_t thermal_image_int[PX_Y*INT_POINTS][PX_X*INT_POINTS];

uint8_t scaleTemp(float temp, const float scaledMax); // scaled min is 0

//----------------------------------------------------------------------
void setImagePixel(uint8_t x_coord, uint8_t y_coord, float temp)
{
	uint32_t value=scaleTemp(temp,255.0f);
	thermal_image[y_coord][x_coord]=value;
}

uint8_t getImagePixel(uint8_t x_coord, uint8_t y_coord)
{
	if(interpolate)
		return thermal_image_int[y_coord][x_coord];
	else
		return thermal_image[y_coord][x_coord];

}
//----------------------------------------------------------------------
void setInterpolate(uint8_t intp)
{
	interpolate=intp;
}

uint8_t getInterpolate(void)
{
	return interpolate;
}
//----------------------------------------------------------------------
void setAutoscale(uint8_t as)
{
	autoscale=as;
}

uint8_t getAutoscale(void)
{
	return autoscale;
}
//----------------------------------------------------------------------
void setColorMode(uint8_t cm)
{
	mode=cm;
}
//----------------------------------------------------------------------
uint8_t getColorMode(void)
{
	return mode;
}
//----------------------------------------------------------------------
void setMinTemp(float minT)
{
	minTemp=minT;
}

float getMinTemp(void)
{
	return minTemp;
}
//----------------------------------------------------------------------
void setMaxTemp(float maxT)
{
	maxTemp=maxT;
}

float getMaxTemp(void)
{
	return maxTemp;
}
//----------------------------------------------------------------------
uint32_t getPixelColor(uint8_t value)
{
	uint32_t rgb24=0;
	switch(mode)
	{
		case 0:
			rgb24=plasma[value];
			break;
		case 1:
			rgb24=jet[value];
			break;
		case 2:
			rgb24=seismic[value];
			break;
		case 3:
			rgb24=hot[value];
			break;
		case 4:
			rgb24=(((value)<<16)|(value<<8)|(value));
			break;
		case 5:
			rgb24=viridis[value];
			break;
		case 6:
			rgb24=hotspot[value];
			break;
		case 7:
			rgb24=coldspot[value];
			break;
		case 8:
			rgb24=hotcold[value];
			break;
	}
	return rgb24;
}
//----------------------------------------------------------------------
void interpolateBilinear(int newWidth, int newHeight)
{
	float a,b,c,d, tmp;
	int x,y,i,j;
	float tx = (float)(32-1)/newWidth;
    float ty = (float)(24-1)/newHeight;
    float x_diff, y_diff;
	for(i=0; i<newHeight; i++)
	{
		for(j=0; j<newWidth; j++)
		{
            x = (int)(tx * j);
            y = (int)(ty * i);

            x_diff = ((tx * j) -(float)x);
            y_diff = ((ty * i) -(float)y );

            a = (float)(thermal_image[y][x]);
            b = (float)(thermal_image[y][x+1]);
            c = (float)(thermal_image[y+1][x]);
            d = (float)(thermal_image[y+1][x+1]);
            tmp=a*(1-x_diff)*(1-y_diff);
        	tmp+=b*(1-y_diff)*(x_diff);
			tmp+=c*(y_diff)*(1-x_diff);
			tmp+=d*(y_diff)*(x_diff);


			thermal_image_int[i][j]=(uint8_t)tmp;
		}
	}
}
//----------------------------------------------------------------------
uint8_t scaleTemp(float temp, const float scaledMax)
{
	float scaled_val=0.0f;

	if(temp < minTemp)
		temp = minTemp;
	else if(temp> maxTemp)
		temp = maxTemp;

	scaled_val = temp-minTemp;
	scaled_val *= scaledMax;
	scaled_val /= (maxTemp-minTemp);

	return (uint8_t)scaled_val;
}
//----------------------------------------------------------------------


