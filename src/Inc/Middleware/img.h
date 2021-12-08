#ifndef IMG_H
#define IMG_H

#include <stdint.h>
#include <math.h>

#define PX_SIZE		12				//size of image pixel
#define PX_X		32				//fixed (MLX90640)
#define PX_Y		24				//fixed (MLX90640)
#define INT_POINTS 	2				//PX_SIZE/INT_POINTS is image pixel size if interpolation is used
#define IMG_X_SIZE	(PX_X*PX_SIZE) 	//32*12
#define IMG_Y_SIZE	(PX_Y*PX_SIZE) 	//24*12


//----------------------------------------------------------------------
void setImagePixel(uint8_t x_coord, uint8_t y_coord, float temp);
uint8_t getImagePixel(uint8_t x_coord, uint8_t y_coord);
//----------------------------------------------------------------------
void setInterpolate(uint8_t intp);
uint8_t getInterpolate(void);
//----------------------------------------------------------------------
void setAutoscale(uint8_t as);
uint8_t getAutoscale(void);
//----------------------------------------------------------------------
void setColorMode(uint8_t cm);
uint8_t getColorMode(void);
//----------------------------------------------------------------------
void setMinTemp(float minT);
float getMinTemp(void);
//----------------------------------------------------------------------
void setMaxTemp(float maxT);
float getMaxTemp(void);
//----------------------------------------------------------------------
uint32_t getPixelColor(uint8_t value);
//----------------------------------------------------------------------
void interpolateBilinear(int newWidth, int newHeight);
//----------------------------------------------------------------------

#endif /* IMG_H */
