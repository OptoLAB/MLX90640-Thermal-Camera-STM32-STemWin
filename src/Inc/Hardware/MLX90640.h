#ifndef MLX90640_H
#define MLX90640_H

#include <stdint.h>
#include "stm32f7xx_hal.h"

//mem adress
#define MLX90640_STAT_REG 	0x8000
#define MLX90640_CTRL_REG1	0x800D
#define MLX90640_I2C_REG 	0x800F
#define MLX90640_RAM 		0x0400
#define MLX90640_EE			0x2400
#define  MLX90640_CTRL_REG1_EE  0x240C

//reg values
#define MLX90640_RES16 		0x00 // 16-bit resolution
#define MLX90640_RES17		0x01 // 17-bit resolution
#define MLX90640_RES18		0x02 // 18-bit resolution
#define MLX90640_RES19		0x03 // 19-bit resolution

#define MLX90640_RATE_05HZ	0x00
#define MLX90640_RATE_1HZ	0x01
#define MLX90640_RATE_2HZ	0x02
#define MLX90640_RATE_4HZ	0x03
#define MLX90640_RATE_8HZ	0x04
#define MLX90640_RATE_16HZ	0x05
#define MLX90640_RATE_32HZ	0x06
#define MLX90640_RATE_64HZ	0x07

#define MLX90640_INTERLEAVE 0
#define MLX90640_CHESS 		1

#define MLX90640_DEFAULT	0x01	// Default -> Subpade mode is activated, Transfer into RAM at each measured frame, Toggles between subpages

//i2c
#define MLX90640_TIMEOUT	1000
#define MLX90640_ADDR		(0x33<<1)

//-------------------------------------------------------------
void Init_MLX90640_GPIO(I2C_HandleTypeDef i2c_handle);
//-------------------------------------------------------------
void MLX90640_I2CRead(uint16_t startAddress, uint16_t nWordsRead, uint16_t *data);
void MLX90640_I2CWrite(uint16_t writeAddress, uint16_t data);
//-------------------------------------------------------------
void MLX90640_SetRefreshRate(uint16_t refreshRate);
uint16_t MLX90640_GetRefreshRate(void);
//-------------------------------------------------------------
void MLX90640_SetResolution(uint16_t resolution);
uint16_t MLX90640_GetResolution(void);
//-------------------------------------------------------------
void MLX90640_SetPattern(uint8_t mode);
uint8_t MLX90640_GetPattern(void);
//-------------------------------------------------------------
void MLX90640_SetMode(uint8_t mode); // sets B3-B0 in Control reg 1
uint8_t MLX90640_GetMode(void);
//-----------------------------------------------------------
uint16_t MLX90640_GetFrameData(uint16_t *frameData);
uint16_t MLX90640_GetSubPageNumber(uint16_t *frameData);
void MLX90640_SynchFrame(void);
//-----------------------------------------------------------
void MLX90640_DumpEE(uint16_t *eeData);
//-----------------------------------------------------------
void MLX90640_ConfigI2C(uint16_t value);
//-----------------------------------------------------------

#endif
