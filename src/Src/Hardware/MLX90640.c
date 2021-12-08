#include "MLX90640.h"

I2C_HandleTypeDef hi2c_mlx;

//------------------------------------------------------------------------------

void Init_MLX90640_GPIO(I2C_HandleTypeDef i2c_handle)
{
	hi2c_mlx=i2c_handle;
}
//------------------------------------------------------------------------------

void MLX90640_I2CRead(uint16_t startAddress, uint16_t nWordsRead, uint16_t *data) //works
{
	uint16_t j,i;
	uint8_t* buf = (uint8_t*) data;

	HAL_I2C_Mem_Read(&hi2c_mlx, MLX90640_ADDR, startAddress, I2C_MEMADD_SIZE_16BIT, buf, nWordsRead<<1, MLX90640_TIMEOUT);

	for(j=0; j<nWordsRead; j++)
	{
		i = j << 1;
		*data++ = ((uint16_t)buf[i]<<8)|((uint16_t)buf[i+1]);
	}

}
//------------------------------------------------------------------------------

void MLX90640_I2CWrite(uint16_t writeAddress, uint16_t data)
{
	uint8_t tx_buff[2] = {data>>8, data & 0x00FF};

    HAL_I2C_Mem_Write(&hi2c_mlx, MLX90640_ADDR, writeAddress, I2C_MEMADD_SIZE_16BIT, tx_buff, 2, MLX90640_TIMEOUT);
}
//------------------------------------------------------------------------------

void MLX90640_SetRefreshRate(uint16_t refreshRate)
{
    uint16_t controlRegister1;
    uint16_t value;

    value = (refreshRate & 0x0007)<<7;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    value = (controlRegister1 & 0xFC7F) | value;
    MLX90640_I2CWrite(MLX90640_CTRL_REG1, value);

}
//------------------------------------------------------------------------------

uint16_t MLX90640_GetRefreshRate(void)
{
    uint16_t controlRegister1;
    int refreshRate;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    refreshRate = (controlRegister1 & 0x0380) >> 7;

    return refreshRate;
}

//------------------------------------------------------------------------------
void MLX90640_SetResolution(uint16_t resolution)
{
    uint16_t controlRegister1;
    uint16_t value;

    value = (resolution & 0x03) << 10;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    value = (controlRegister1 & 0xF3FF) | value;
    MLX90640_I2CWrite(MLX90640_CTRL_REG1, value);
}
//------------------------------------------------------------------------------
uint16_t MLX90640_GetResolution(void)
{
    uint16_t controlRegister1;
    uint16_t resolutionRAM;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    resolutionRAM = (controlRegister1 & 0x0C00) >> 10;

    return resolutionRAM;
}
//------------------------------------------------------------------------------

void MLX90640_SetPattern(uint8_t mode)
{
    uint16_t controlRegister1;
    uint16_t value;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    if(mode)value = (controlRegister1 | 0x1000); 	//set B12
    else value = (controlRegister1 & 0xEFFF);		//reset B12
    MLX90640_I2CWrite(MLX90640_CTRL_REG1, value);
}

//------------------------------------------------------------------------------

uint8_t MLX90640_GetPattern(void)
{
    uint16_t controlRegister1;
    uint8_t modeRAM;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    modeRAM = (controlRegister1 & 0x1000) >> 12;

    return modeRAM;
}
//------------------------------------------------------------------------------

void MLX90640_SetMode(uint8_t mode)
{
    uint16_t controlRegister1;
    uint16_t value=0;

    value = (mode & 0x0F);

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    value = (controlRegister1 & 0xFFF0) | value;
    MLX90640_I2CWrite(MLX90640_CTRL_REG1, value);
}
//------------------------------------------------------------------------------

uint8_t MLX90640_GetMode(void)
{

    uint16_t controlRegister1;
    uint8_t mode;

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    mode = (controlRegister1 & 0x000F);

    return mode;
}
//------------------------------------------------------------------------------

uint16_t MLX90640_GetFrameData(uint16_t *frameData)
{
    uint16_t dataReady = 0;
    uint16_t controlRegister1;
    uint16_t statusRegister;

    while(dataReady == 0)
    {
        MLX90640_I2CRead(MLX90640_STAT_REG, 1, &statusRegister);
        dataReady = statusRegister & 0x0008;
    }

    MLX90640_I2CWrite(MLX90640_STAT_REG, 0x0030);
    MLX90640_I2CRead(MLX90640_RAM, 832, frameData);
    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    frameData[832] = controlRegister1;
    frameData[833] = statusRegister & 0x0001;


    return frameData[833];
}
//------------------------------------------------------------------------------

uint16_t MLX90640_GetSubPageNumber(uint16_t *frameData)
{
    return frameData[833];

}
//------------------------------------------------------------------------------

void MLX90640_SynchFrame(void)
{
    uint16_t dataReady = 0;
    uint16_t statusRegister;

    MLX90640_I2CWrite(MLX90640_STAT_REG, 0x0030);

    while(dataReady == 0)
    {
        MLX90640_I2CRead(MLX90640_STAT_REG, 1, &statusRegister);
        dataReady = statusRegister & 0x0008;
    }
}
//------------------------------------------------------------------------------

void MLX90640_DumpEE(uint16_t *eeData)
{
    MLX90640_I2CRead(MLX90640_EE, 832, eeData);
}
//------------------------------------------------------------------------------



void MLX90640_ConfigI2C(uint16_t value)
{
    uint16_t i2c_reg;

    MLX90640_I2CRead(MLX90640_I2C_REG, 1, &i2c_reg);
    i2c_reg = (i2c_reg & 0xFFF8)|value; //reset B2 B1 B0 then set

    MLX90640_I2CWrite(MLX90640_I2C_REG, i2c_reg);

}
/*
int MLX90640_GetFrameData(uint16_t *frameData)//works
{
    uint16_t dataReady = 1;
    uint16_t controlRegister1;
    uint16_t statusRegister;
    uint8_t cnt = 0;

    dataReady = 0;
    MLX90640_I2CWrite(MLX90640_STAT_REG, 0x0030);
    while(dataReady == 0)
    {
        MLX90640_I2CRead(MLX90640_STAT_REG, 1, &statusRegister);
        dataReady = statusRegister & 0x0008;
    }

    while(dataReady != 0 && cnt < 5)
    {
        MLX90640_I2CWrite(MLX90640_STAT_REG, 0x0030);
        MLX90640_I2CRead(MLX90640_RAM, 832, frameData);
        MLX90640_I2CRead(MLX90640_STAT_REG, 1, &statusRegister);
        dataReady = statusRegister & 0x0008;
        cnt = cnt + 1;
    }

    if(cnt > 4)
    {
        return -8;
    }

    MLX90640_I2CRead(MLX90640_CTRL_REG1, 1, &controlRegister1);
    frameData[832] = controlRegister1;
    frameData[833] = statusRegister & 0x0001;

    return frameData[833];
}*/
//------------------------------------------------------------------------------

