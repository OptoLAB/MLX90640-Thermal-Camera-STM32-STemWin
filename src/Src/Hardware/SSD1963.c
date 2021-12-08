/*
J. Bajic - 2018.
*/

#include "SSD1963.h"

//=============================================================================
// GPIO setup 
//=============================================================================
void Init_LCD_GPIO(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

	  __HAL_RCC_CRC_CLK_ENABLE();
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();


	  HAL_GPIO_WritePin(GDISP_CMD_PORT, GCMD, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GDISP_DATA_LO_PORT, GDATA_LO, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GDISP_DATA_HI_PORT, GDATA_HI, GPIO_PIN_RESET);

	  GPIO_InitStruct.Pin = GCMD;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GDISP_CMD_PORT, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GDATA_LO;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GDISP_DATA_LO_PORT, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GDATA_HI;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GDISP_DATA_HI_PORT, &GPIO_InitStruct);

	  SET_CS; SET_WR; SET_RD; SET_DC; SET_RST;
}
//=============================================================================
// Write command to display
//=============================================================================
void SSD1963_WriteCommand(uint16_t Data)
{
	uint16_t _Data=~Data;

	GDISP_DATA_HI_PORT->BSRR=(uint32_t)(0xFF00)<<16;

	CLR_CS;
	CLR_DC;
	GDISP_DATA_LO_PORT->BSRR=((uint32_t)(_Data & GDATA_LO)<<16)|(Data & GDATA_LO);
	GDISP_DATA_HI_PORT->ODR|=(Data & GDATA_HI);
	writeStrobe;
	SET_CS;
}
//=============================================================================
// Write data to display
//=============================================================================

void SSD1963_WriteData(uint16_t Data)
{
	uint16_t _Data=~Data;

	GDISP_DATA_HI_PORT->BSRR=(uint32_t)(0xFF00)<<16;

	CLR_CS;
	SET_DC;
	GDISP_DATA_LO_PORT->BSRR=((uint32_t)(_Data & GDATA_LO)<<16)|(Data & GDATA_LO);
	GDISP_DATA_HI_PORT->ODR|=(Data & GDATA_HI);
	writeStrobe;
	SET_CS;

}
//=============================================================================
// Write multiple data to display
//=============================================================================
void SSD1963_WriteDataMultiple(uint16_t * Data, int NumItems)
{
  while (NumItems)
  {
	  SSD1963_WriteData(*Data);
	  Data++;
	  NumItems--;
  }
}
//=============================================================================
// Read data from display - NOT IMPLEMENTED
//=============================================================================
uint16_t SSD1963_ReadData(void)
{
  return 0;
}

//=============================================================================
// Read multiple data from display - NOT IMPLEMENTED
//=============================================================================
void SSD1963_ReadDataMultiple(uint16_t * Data, int NumItems)
{
  while (NumItems)
  {
	  NumItems--;

  }
}
//=============================================================================
// Display initialization
//=============================================================================

void SSD1963_Init (void)
{
	CLR_RST;  // reset
	HAL_Delay(500);
	SET_RST;
	HAL_Delay(100);

	SSD1963_WriteCommand(SSD1963_SOFT_RESET);
	HAL_Delay(10);
	SSD1963_WriteCommand(SSD1963_SET_PLL_MN);
	SSD1963_WriteData(29);	// PLLclk = REFclk * 30 (300MHz)
	SSD1963_WriteData(2);	// SYSclk = PLLclk / 3  (100MHz)
	SSD1963_WriteData(4);  	// dummy

	SSD1963_WriteCommand(SSD1963_SET_PLL);
	SSD1963_WriteData(0x0001);

	HAL_Delay(100);

	SSD1963_WriteCommand(SSD1963_SET_PLL);
	SSD1963_WriteData(0x0003);

	SSD1963_WriteCommand(SSD1963_SET_LCD_MODE);
	SSD1963_WriteData(0x0020);
	SSD1963_WriteData(0x0000);
	SSD1963_WriteData(mHIGH((TFT_WIDTH-1)));
	SSD1963_WriteData(mLOW((TFT_WIDTH-1)));
	SSD1963_WriteData(mHIGH((TFT_HEIGHT-1)));
	SSD1963_WriteData(mLOW((TFT_HEIGHT-1)));
	SSD1963_WriteData(0x0000);

	SSD1963_WriteCommand(SSD1963_SET_PIXEL_DATA_INTERFACE);
	SSD1963_WriteData(SSD1963_PDI_16BIT565);

	SSD1963_WriteCommand(SSD1963_SET_LSHIFT_FREQ);
	SSD1963_WriteData((LCD_FPR >> 16) & 0x00FF);
	SSD1963_WriteData((LCD_FPR >> 8) & 0x00FF);
	SSD1963_WriteData(LCD_FPR & 0x00FF);

	SSD1963_WriteCommand(SSD1963_SET_HORI_PERIOD);
	SSD1963_WriteData(mHIGH(TFT_HSYNC_PERIOD));
	SSD1963_WriteData(mLOW(TFT_HSYNC_PERIOD));
	SSD1963_WriteData(mHIGH((TFT_HSYNC_PULSE + TFT_HSYNC_BACK_PORCH)));
	SSD1963_WriteData(mLOW((TFT_HSYNC_PULSE + TFT_HSYNC_BACK_PORCH)));
	SSD1963_WriteData(TFT_HSYNC_PULSE);
	SSD1963_WriteData(0x0000);
	SSD1963_WriteData(0x0000);
	SSD1963_WriteData(0x0000);

	SSD1963_WriteCommand(SSD1963_SET_VERT_PERIOD);
	SSD1963_WriteData(mHIGH(TFT_VSYNC_PERIOD));
	SSD1963_WriteData(mLOW(TFT_VSYNC_PERIOD));
	SSD1963_WriteData(mHIGH((TFT_VSYNC_PULSE + TFT_VSYNC_BACK_PORCH)));
	SSD1963_WriteData(mLOW((TFT_VSYNC_PULSE + TFT_VSYNC_BACK_PORCH)));
	SSD1963_WriteData(TFT_VSYNC_PULSE);
	SSD1963_WriteData(0x0000);
	SSD1963_WriteData(0x0000);

	SSD1963_WriteCommand(SSD1963_SET_DISPLAY_ON);//SET display on

	HAL_Delay(10);
	SSD1963_WriteCommand(SSD1963_SET_PWM_CONF); //set PWM for Backlight. Manual p.53
	// 6 parameters to be set
	SSD1963_WriteData(0x0004); 					// PWM Freq =100MHz/(256*(PWMF[7:0]+1))/256  PWMF[7:0]=4 PWM Freq=305Hz
	SSD1963_WriteData(0x00C0); 					// PWM duty cycle(50%)
	SSD1963_WriteData(0x0001); 					// PWM controlled by host, PWM enabled
	SSD1963_WriteData(0x0040); 					// brightness level 0x00 - 0xFF
	SSD1963_WriteData(0x0000); 					// minimum brightness level =  0x00 - 0xFF
	SSD1963_WriteData(0x0000);					// brightness prescalar 0x0 - 0xF


}
//=============================================================================
// Selects screen area
//=============================================================================
void SSD1963_SetArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey)
{
	SSD1963_WriteCommand(SSD1963_SET_COLUMN_ADDRESS);
	SSD1963_WriteData((sx >> 8) & 0xFF);
	SSD1963_WriteData((sx >> 0) & 0xFF);
	SSD1963_WriteData((ex >> 8) & 0xFF);
	SSD1963_WriteData((ex >> 0) & 0xFF);

	SSD1963_WriteCommand(SSD1963_SET_PAGE_ADDRESS);
	SSD1963_WriteData((sy >> 8) & 0xFF);
	SSD1963_WriteData((sy >> 0) & 0xFF);
	SSD1963_WriteData((ey >> 8) & 0xFF);
	SSD1963_WriteData((ey >> 0) & 0xFF);
}
//=============================================================================
// Fill area with specified color
//=============================================================================
void SSD1963_FillArea(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, uint16_t color)
{
	uint16_t i;
	SSD1963_SetArea(sx, ex, sy, ey);
	SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);
	for(i = 0; i < ((ex-sx+1)*(ey-sy+1)); i++)
	{
	SSD1963_WriteData(color);
	}
}
//=============================================================================
// Fills entire screen with specified color
//=============================================================================
void SSD1963_ClearScreen(uint16_t color)
{
	uint16_t x,y;
	SSD1963_SetArea(0, TFT_WIDTH-1 , 0, TFT_HEIGHT-1);
	SSD1963_WriteCommand(0x2c);
	for(x=0;x<TFT_WIDTH;x++)
	{
        for(y= 0;y<TFT_HEIGHT;y++)
                {
                SSD1963_WriteData(color);
                }
	}
}
//=============================================================================
// Sets given pixel with specified color
//=============================================================================
void SSD193_SetPixel(uint16_t x, uint16_t y, uint16_t color)
{
	SSD1963_SetArea(x, x, y, y);
	SSD1963_WriteCommand(0x2c);
	SSD1963_WriteData(color);
	SSD1963_WriteCommand(0x0);
}
//=============================================================================
// Controls backlight
//=============================================================================
void SSD1963_SetBacklight(uint8_t BL)
{
	SSD1963_WriteCommand(SSD1963_SET_PWM_CONF); 		//set PWM for Backlight. Manual p.53
	// 6 parameters to be set
	SSD1963_WriteData(0x0004); 							// PWM Freq =100MHz/(256*(PWMF[7:0]+1))/256  PWMF[7:0]=4 PWM Freq=305Hz
	SSD1963_WriteData((uint16_t)BL); 					// PWM duty cycle(50%)
	SSD1963_WriteData(0x0001); 							// PWM controlled by host, PWM enabled
	SSD1963_WriteData(0x0040); 							// brightness level 0x00 - 0xFF
	SSD1963_WriteData(0x0000); 							// minimum brightness level =  0x00 - 0xFF
	SSD1963_WriteData(0x0000);							// brightness prescalar 0x0 - 0xF
}
//=============================================================================
// Sets scroll area
//=============================================================================
void SSD1963_SetScrollArea(uint16_t top, uint16_t scroll, uint16_t bottom)
{
	SSD1963_WriteCommand(SSD1963_SET_SCROLL_AREA);
	SSD1963_WriteData(top>>8);
	SSD1963_WriteData(top);
	SSD1963_WriteData(scroll>>8);
	SSD1963_WriteData(scroll);
	SSD1963_WriteData(bottom>>8);
	SSD1963_WriteData(bottom);
}
//=============================================================================
// Sets scroll area start
//=============================================================================
void SSD1963_SetScrollStart(uint16_t line)
{
	SSD1963_WriteCommand(SSD1963_SET_SCROLL_START);
	SSD1963_WriteData(line>>8);
	SSD1963_WriteData(line);
}
