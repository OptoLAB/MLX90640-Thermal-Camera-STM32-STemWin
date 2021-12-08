/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//STemWin
#include "WM.h"
#include "GUI.h"
//Hardware
#include "SSD1963.h"
#include "STMPE610.h"
#include "MLX90640.h"

//Middleware
#include "MLX90640_API.h"
#include "bmp.h"
#include "img.h"
//Windows
#include "MainWindowDLG.h"
#include "SettingsDLG.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
#define MAIN 0
#define RUN_SETTINGS 1
#define SETTINGS 2
#define CANCEL 3
#define SAVE 4

#define TA_SHIFT 8
float mlx90640To[768];
uint16_t eeMLX90640[832];
paramsMLX90640 mlx90640;
uint16_t mlx90640Frame[834];
int status=0;

int keyPressed = 0;		// variable for ID of touched element
uint8_t menu=MAIN;
char value[5];
int val=0;

WM_HWIN hWin;
WM_HWIN hWin1;

FATFS myFAT;
FIL myFile;
FRESULT res;

UINT byteCount;

uint8_t capture=0;
/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

extern volatile GUI_TIMER_TIME OS_TimeMS;
void HAL_SYSTICK_Callback(void)
{
	OS_TimeMS++;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  //init GPIO
  Init_LCD_GPIO();
  Init_TOUCH_GPIO(hi2c1);
  Init_MLX90640_GPIO(hi2c2);

  hi2c2.Init.Timing = 0x00501E63; //FM !!! must be change if clk is changed
  HAL_I2C_Init(&hi2c2);


  MLX90640_SetRefreshRate(MLX90640_RATE_8HZ);
  MLX90640_SetResolution(MLX90640_RES16);
  MLX90640_SetPattern(MLX90640_CHESS);
  MLX90640_SetMode(MLX90640_DEFAULT);

  MLX90640_DumpEE(eeMLX90640);
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);

  HAL_Delay(10);

  //init GUI
  WM_SetCreateFlags(WM_CF_MEMDEV);
  GUI_Init();
  GUI_Clear();

  hWin = CreateMainWindow();
  invalidateLegend(hWin); //triggers legend drawing

  GUI_Exec(); //draw base screen

  hi2c2.Init.Timing = 0x00200E22; //FM+ !!! must be change if clk is changed
  HAL_I2C_Init(&hi2c2);
  HAL_Delay(10);

  //UNLINK PATH
  FATFS_UnLinkDriver(SDPath);
  HAL_Delay(10);

  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
	  STMPE610_read_xyz();

	  switch(menu)
	  {
			  case MAIN:
				  status = MLX90640_GetFrameData(mlx90640Frame);

				  float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
				  float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
				  float emissivity = 0.95;
				  MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);
				  MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, mlx90640To, 0, &mlx90640);
				  MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, mlx90640To, 0, &mlx90640);

				  int i,j,k=0;
				  float varMaxTempVal=0.0, varMinTempVal=300;
				  for(i=0; i<24; i++)
				  {
					  for(j=0; j<32; j++)
					  {
						  float tmp = mlx90640To[k];
						  if(tmp > varMaxTempVal)
							  varMaxTempVal = (tmp); //find max Temp pixel
						  if(tmp < varMinTempVal)
							  varMinTempVal = (tmp); //find maxTemp pixel
						  setImagePixel(j,i,tmp);
						  k++;
					  }
				  }

				  if(getAutoscale())
				  {
					  if(varMaxTempVal==varMinTempVal)varMaxTempVal+=1; // to avoid zero division
					  setMaxTemp(varMaxTempVal);
					  setMinTemp(varMinTempVal);
					  updateLabels(hWin, varMinTempVal, varMaxTempVal);
				  }
				  
				  if(getInterpolate())interpolateBilinear(PX_X*INT_POINTS, PX_Y*INT_POINTS);
				  invalidateImage(hWin);	//triggers image drawing
				  break;
			  case RUN_SETTINGS:
				  hWin1 = CreateSettings();
				  SLIDER_SetValue(WM_GetDialogItem(hWin1, ID_SLIDER_0), getMinTemp());
				  SLIDER_SetValue(WM_GetDialogItem(hWin1, ID_SLIDER_1), getMaxTemp());
				  DROPDOWN_SetSel(WM_GetDialogItem(hWin1, ID_DROPDOWN_0), getColorMode());
				  menu=SETTINGS;
				  break;
			  case SETTINGS:
				   val=SPINBOX_GetValue(WM_GetDialogItem(hWin1, ID_SPINBOX_1));
				   SLIDER_SetValue(WM_GetDialogItem(hWin1, ID_SLIDER_1), val);
				   val=SPINBOX_GetValue(WM_GetDialogItem(hWin1, ID_SPINBOX_0));
				   SLIDER_SetValue(WM_GetDialogItem(hWin1, ID_SLIDER_0), val);
		  		  break;
			  case CANCEL:
				  WM_DeleteWindow(hWin1);
				  menu=MAIN;
				  break;
			  case SAVE:
				  if(CHECKBOX_IsChecked(WM_GetDialogItem(hWin1,ID_CHECKBOX_0))) setAutoscale(1);
				  else
				  {
			  		  setMinTemp(SLIDER_GetValue(WM_GetDialogItem(hWin1, ID_SLIDER_0)));
			  		  setMaxTemp(SLIDER_GetValue(WM_GetDialogItem(hWin1, ID_SLIDER_1)));
			  		  updateLabels(hWin, getMinTemp(), getMaxTemp());
			  		  setAutoscale(0);
				  }
				  setColorMode(DROPDOWN_GetSel(WM_GetDialogItem(hWin1, ID_DROPDOWN_0)));
				  if(CHECKBOX_IsChecked(WM_GetDialogItem(hWin1,ID_CHECKBOX_1))) setInterpolate(1);
				  else setInterpolate(0);
				  invalidateLegend(hWin);
				  WM_DeleteWindow(hWin1);
				  menu=MAIN;
				  break;

	}
  	GUI_Exec();
  	if(capture)
  	{
  		HAL_TIM_Base_Stop_IT(&htim3); //stop timer while writing to SD card
  		capture=0;
  		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)==GPIO_PIN_RESET)
  	    {
  			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);
  			FATFS_LinkDriver(&SD_Driver, SDPath);
  			res=f_mount(&myFAT, SDPath, 1);
  			captureBMPImage();
  			f_mount(0, SDPath, 1);
  			FATFS_UnLinkDriver(SDPath);
  			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);
  	    }
  		HAL_TIM_Base_Start_IT(&htim3);//restart timer
  	}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20404768;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00200E22;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Enable Fast Mode Plus 
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 27;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	GUI_TOUCH_Exec();
	keyPressed = GUI_GetKey();  //get touched element

	if((keyPressed == ID_BUTTON_2)&&(menu==MAIN))
		menu=RUN_SETTINGS;
	else if((keyPressed == ID_BUTTON_0)&&(menu==SETTINGS))
		menu=CANCEL;
	else if((keyPressed == ID_BUTTON_1)&&(menu==SETTINGS))
		menu=SAVE;
	if((keyPressed == ID_BUTTON_3)&&(menu==MAIN))
		capture=1;

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
