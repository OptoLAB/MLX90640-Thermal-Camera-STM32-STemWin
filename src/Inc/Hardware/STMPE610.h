/*
J. Bajic - 2018
*/
 
#ifndef STMPE610_H
#define STMPE610_H

#include "stm32f7xx_hal.h"

// Identification registers
#define STMPE610_REG_CHP_ID         0x00 // 16-bit
#define STMPE610_REG_ID_VER         0x02

// System registers
#define STMPE610_REG_SYS_CTRL1      0x03
#define STMPE610_REG_SYS_CTRL2      0x04
#define STMPE610_REG_SPI_CFG        0x08

// Interrupt control registers
#define STMPE610_REG_INT_CTRL       0x09
#define STMPE610_REG_INT_EN         0x0A
#define STMPE610_REG_INT_STA        0x0B
#define STMPE610_REG_GPIO_INT_EN    0x0C
#define STMPE610_REG_GPIO_INT_STA   0x0D
#define STMPE610_REG_ADC_INT_EN     0x0E
#define STMPE610_REG_ADC_INT_STA    0x0F

// GPIO registers
#define STMPE610_REG_GPIO_SET_PIN   0x10
#define STMPE610_REG_GPIO_CLR_PIN   0x11
#define STMPE610_REG_GPIO_MP_STA    0x12
#define STMPE610_REG_GPIO_DIR       0x13
#define STMPE610_REG_GPIO_ED        0x14
#define STMPE610_REG_GPIO_RE        0x15
#define STMPE610_REG_GPIO_FE        0x16
#define STMPE610_REG_GPIO_AF        0x17

// ADC registers
#define STMPE610_REG_ADC_CTRL1      0x20
#define STMPE610_REG_ADC_CTRL2      0x21
#define STMPE610_REG_ADC_CAPT       0x22
#define STMPE610_REG_ADC_DATA_CH0   0x30 // 16-bit
#define STMPE610_REG_ADC_DATA_CH1   0x32 // 16-bit
#define STMPE610_REG_ADC_DATA_CH4   0x38 // 16-bit
#define STMPE610_REG_ADC_DATA_CH5   0x3A // 16-bit
#define STMPE610_REG_ADC_DATA_CH6   0x3C // 16-bit
#define STMPE610_REG_ADC_DATA_CH7   0x3E // 16-bit

// Touchscreen registers
#define STMPE610_REG_TSC_CTRL       0x40
#define STMPE610_REG_TSC_CFG        0x41
#define STMPE610_REG_WDW_TR_X       0x42 // 16-bit
#define STMPE610_REG_WDW_TR_Y       0x44 // 16-bit
#define STMPE610_REG_WDW_BL_X       0x46 // 16-bit
#define STMPE610_REG_WDW_BL_Y       0x48 // 16-bit
#define STMPE610_REG_FIFO_TH        0x4A
#define STMPE610_REG_FIFO_STA       0x4B
#define STMPE610_REG_FIFO_SIZE      0x4C
#define STMPE610_REG_TSC_DATA_X     0x4D // 16-bit
#define STMPE610_REG_TSC_DATA_Y     0x4F // 16-bit
#define STMPE610_REG_TSC_DATA_Z     0x51
#define STMPE610_REG_TSC_DATA_XYZ   0x52 // 32-bit
#define STMPE610_REG_TSC_FRACT_XYZ  0x56
#define STMPE610_REG_TSC_DATA       0x57
#define STMPE610_REG_TSC_I_DRIVE    0x58
#define STMPE610_REG_TSC_SHIELD     0x59
// Slave address
#define STMPE610_ADDR				(0x44 << 1)
// Maximum timeout
#define STMPE610_TIMEOUT			0x3000

#define TRUE 1
#define FALSE 0

// Calibration values
#define GUI_TOUCH_AD_TOP 	390
#define GUI_TOUCH_AD_BOTTOM 3977
#define GUI_TOUCH_AD_LEFT 	75
#define GUI_TOUCH_AD_RIGHT 	3955
#define GUI_TOUCH_AD_PMAX 	250
#define GUI_TOUCH_AD_PMIN 	90

// functions
void Init_TOUCH_GPIO(I2C_HandleTypeDef hi2c);

void STMPE610_Init(void);

uint8_t STMPE610_IsTouched(void);
void STMPE610_Write(uint8_t reg, uint8_t val);
uint8_t STMPE610_ReadByte(uint8_t reg);
uint16_t STMPE610_ReadWord(uint8_t reg);

uint8_t STMPE610_read_xyz(void);

uint16_t STMPE610_GetX_TOUCH(void);
uint16_t STMPE610_GetY_TOUCH(void);
uint16_t STMPE610_GetZ_TOUCH(void);

uint16_t STMPE610_GetX_TOUCH_RAW(void);
uint16_t STMPE610_GetY_TOUCH_RAW(void);
uint16_t STMPE610_GetZ_TOUCH_RAW(void);

#endif /* STMPE610_H */


