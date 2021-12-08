/*
J. Bajic - 2018
*/
 
#include "STMPE610.h"


uint16_t x=0, y=0, z=0;

I2C_HandleTypeDef hi2c;


void Init_TOUCH_GPIO(I2C_HandleTypeDef i2c_handle)
{
	hi2c=i2c_handle;
}


uint8_t STMPE610_IsTouched(void)
{
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)!=GPIO_PIN_SET)
		return TRUE;
	else
		return FALSE;
}


void STMPE610_Write(uint8_t reg, uint8_t val)
{
	uint8_t	txbuf[2];

	txbuf[0]=reg;
	txbuf[1]=val;

	while(HAL_I2C_Master_Transmit(&hi2c, STMPE610_ADDR, (uint8_t *)txbuf, 2, STMPE610_TIMEOUT)!= HAL_OK);
}

uint8_t STMPE610_ReadByte(uint8_t reg)
{
	uint8_t rxbuf[1];
	rxbuf[0] = 0;

	while(HAL_I2C_Master_Transmit(&hi2c, STMPE610_ADDR, &reg, 1, STMPE610_TIMEOUT)!= HAL_OK);
	while(HAL_I2C_Master_Receive(&hi2c, STMPE610_ADDR, (uint8_t *)rxbuf, 1, STMPE610_TIMEOUT) != HAL_OK);

	return rxbuf[0];
}

uint16_t STMPE610_ReadWord(uint8_t reg)
{
	uint8_t	rxbuf[2];
	
	rxbuf[0] = 0;
	rxbuf[1] = 0;

	while(HAL_I2C_Master_Transmit(&hi2c, STMPE610_ADDR, &reg, 1, 10000)!= HAL_OK);
	while(HAL_I2C_Master_Receive(&hi2c, STMPE610_ADDR, (uint8_t *)rxbuf, 2, 10000) != HAL_OK);

	return (((uint16_t)rxbuf[0]) << 8) | rxbuf[1];
}

void STMPE610_Init(void)
{
	STMPE610_Write(STMPE610_REG_SYS_CTRL1, 0x02);		// Software chip reset
	HAL_Delay(10);
	
	STMPE610_Write(STMPE610_REG_SYS_CTRL2, 0x04);		// Temperature sensor clock off, GPIO clock off, touch clock on, ADC clock on
	STMPE610_Write(STMPE610_REG_INT_EN, 0x03);			// Interrupt on INT pin when there is a sample or a touch transition.
	//STMPE610_write_reg(STMPE610_REG_INT_EN, 0x00);						// Don't Interrupt on INT pin
	STMPE610_Write(STMPE610_REG_ADC_CTRL1, 0x48);		// ADC conversion time = 80 clock ticks, 12-bit ADC, internal voltage refernce
	HAL_Delay(2);
	
	STMPE610_Write(STMPE610_REG_ADC_CTRL2, 0x01);		// ADC speed 3.25MHz
	STMPE610_Write(STMPE610_REG_GPIO_AF, 0x00);			// GPIO alternate function - OFF
	STMPE610_Write(STMPE610_REG_TSC_CFG, 0xA3);			// Averaging 4, touch detect delay 1ms, panel driver settling time 1ms
	STMPE610_Write(STMPE610_REG_FIFO_TH, 0x01);			// FIFO threshold = 1
	STMPE610_Write(STMPE610_REG_FIFO_STA, 0x01);			// FIFO reset enable
	STMPE610_Write(STMPE610_REG_FIFO_STA, 0x00);			// FIFO reset disable
	STMPE610_Write(STMPE610_REG_TSC_FRACT_XYZ, 0x07);	// Z axis data format
	STMPE610_Write(STMPE610_REG_TSC_I_DRIVE, 0x01);		// max 50mA touchscreen line current

	STMPE610_Write(STMPE610_REG_TSC_CTRL, 0x30);			// X&Y&Z, 16 reading window
	STMPE610_Write(STMPE610_REG_TSC_CTRL, 0x31);			// X&Y&Z, 16 reading window, TSC enable
	//STMPE610_write_reg(STMPE610_REG_TSC_CTRL, 0x32);						// X&Y, 16 reading window
	//STMPE610_write_reg(TMPE610_REG_TSC_CTRL, 0x33);						// X&Y, 16 reading window, TSC enable

	STMPE610_Write(STMPE610_REG_INT_STA, 0xFF);		// Clear all interrupts
	STMPE610_Write(STMPE610_REG_INT_CTRL, 0x01);		// Level interrupt, enable interrupts
}

uint8_t STMPE610_read_xyz(void)
{
	uint8_t	status;

	// Is there something in the fifo
	status = STMPE610_ReadByte(STMPE610_REG_FIFO_STA);
	if ((status & 0x20))
	{
		STMPE610_Write(STMPE610_REG_INT_STA, 0xFF);
		if (!(STMPE610_ReadByte(STMPE610_REG_TSC_CTRL) & 0x80))
		{
			x=0;
			y=0;
			z=0;
			return TRUE;
		}
		return FALSE;
	}

	// Time to get some readings
	x = STMPE610_ReadWord(STMPE610_REG_TSC_DATA_X);
	y = STMPE610_ReadWord(STMPE610_REG_TSC_DATA_Y);
	z = STMPE610_ReadByte(STMPE610_REG_TSC_DATA_Z);

	STMPE610_Write(STMPE610_REG_FIFO_STA, 0x01);		// FIFO reset enable
	STMPE610_Write(STMPE610_REG_FIFO_STA, 0x00);		// FIFO reset disable
	STMPE610_Write(STMPE610_REG_INT_STA, 0xFF);

	return TRUE;
}

uint16_t STMPE610_GetX_TOUCH(void)
{
	float pom=0;

	pom=(float)x;
	pom=800.0*(pom-GUI_TOUCH_AD_LEFT)/(GUI_TOUCH_AD_RIGHT-GUI_TOUCH_AD_LEFT);
	return (uint16_t)pom;
}
uint16_t STMPE610_GetY_TOUCH(void)
{
	float pom=0;

	pom=(float)y;
	pom=480.0*(pom-GUI_TOUCH_AD_TOP)/(GUI_TOUCH_AD_BOTTOM-GUI_TOUCH_AD_TOP);
	return (uint16_t)pom;
}
uint16_t STMPE610_GetZ_TOUCH(void)
{
	float pom=0;

	pom=(float)z;
	pom=100*(pom-GUI_TOUCH_AD_PMIN)/(GUI_TOUCH_AD_PMAX-GUI_TOUCH_AD_PMIN);
	return (uint16_t)pom;
}

uint16_t STMPE610_GetX_TOUCH_RAW(void)
{
	return x;
}
uint16_t STMPE610_GetY_TOUCH_RAW(void)
{
	return y;
}
uint16_t STMPE610_GetZ_TOUCH_RAW(void)
{
	return z;
}



