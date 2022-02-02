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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "SSD1963.h"
#include "STMPE610.h"

#include "GUI.h"
#include "LCDConf.h"

#include <stdint.h>
#include <stdio.h>
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern volatile GUI_TIMER_TIME OS_TimeMS;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define HOURS_TYPE 0
#define MINUTES_TYPE 1
#define SECONDS_TYPE 2

#define DATE_TYPE 0
#define MONTH_TYPE 1
#define YEAR_TYPE 2
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

GUI_MEMDEV_Handle hMem, hMem_clock, hMem_temp;

uint32_t ADC_BUF[1], temperatura_ADC_sirovo;

float angle = 0;
int x_coordinate = 0;
int y_coordinate = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC3_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

int get_time(int type);
int get_date(int type);
int get_temp(void);
void draw_display(void);
void draw_clock(void);
void draw_temperature(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_SYSTICK_Callback(void)
{
	OS_TimeMS++;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)  //ova fja se pozove kad se izvrsi jedna konverzija
{
	if(hadc->Instance == ADC3)
	{
		temperatura_ADC_sirovo = ADC_BUF[0];
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_ADC3_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc3,ADC_BUF,1);
  Init_LCD_GPIO();
  Init_TOUCH_GPIO(hi2c1);
  STMPE610_Init();

  GUI_Init();


  draw_display();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	draw_clock();

	draw_temperature();

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

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

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
  hi2c1.Init.Timing = 0x20303E5D;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x59;
  sTime.Seconds = 0x55;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

int get_time(int type)
{
	RTC_TimeTypeDef gTime;

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);


	if(type == HOURS_TYPE)
		return gTime.Hours;
	if(type == MINUTES_TYPE)
		return gTime.Minutes;
	return gTime.Seconds;
}

int get_date(int type)
{
	RTC_DateTypeDef gDate;

	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);


	if(type == DATE_TYPE)
		return gDate.Date;
	if(type == MONTH_TYPE)
		return gDate.Month;
	return gDate.Year;
}

int get_temp(void)
{
	float temperatura = 0.0;

	temperatura = (100.0*(((float)temperatura_ADC_sirovo)/(2048.0)-0.5));
	temperatura = (int)temperatura;


	return temperatura;
}

void draw_display(void)
{
	hMem = GUI_MEMDEV_Create(0,0,480,272);

	GUI_MEMDEV_Select(hMem);
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_SetColor(GUI_BLACK);
	GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	GUI_SetPenSize( 5 );
	GUI_DrawArc(136, 136, 130, 130, -90, 280);

	GUI_SetPenSize( 8 );
	angle = 90;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //3

	angle = 270;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //9

	angle = 0;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_SetFont(&GUI_Font16B_ASCII);
	GUI_DispStringHCenterAt("XII", x_coordinate, y_coordinate-5);

	angle = 180;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //6

	GUI_SetPenSize( 6 );
	angle = 60;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //2

	angle = 300;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //10

	angle = 120;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //4

	angle = 240;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //8

	angle = 30;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //1

	angle = 150;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //5

	angle = 330;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //11

	angle = 210;
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*120));
	y_coordinate=(136-(cos(angle)*120));
	GUI_DrawPoint(x_coordinate, y_coordinate); //7


	GUI_SetFont(&GUI_Font13_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt("00 - 01 :", 350, 10);
	GUI_DispStringHCenterAt("01 - 02 :", 350, 20);
	GUI_DispStringHCenterAt("02 - 03 :", 350, 30);
	GUI_DispStringHCenterAt("03 - 04 :", 350, 40);
	GUI_DispStringHCenterAt("04 - 05 :", 350, 50);
	GUI_DispStringHCenterAt("05 - 06 :", 350, 60);
	GUI_DispStringHCenterAt("06 - 07 :", 350, 70);
	GUI_DispStringHCenterAt("07 - 08 :", 350, 80);
	GUI_DispStringHCenterAt("08 - 09 :", 350, 90);
	GUI_DispStringHCenterAt("09 - 10 :", 350, 100);
	GUI_DispStringHCenterAt("10 - 11 :", 350, 110);
	GUI_DispStringHCenterAt("11 - 12 :", 350, 120);
	GUI_DispStringHCenterAt("12 - 13 :", 350, 130);
	GUI_DispStringHCenterAt("13 - 14 :", 350, 140);
	GUI_DispStringHCenterAt("14 - 15 :", 350, 150);
	GUI_DispStringHCenterAt("15 - 16 :", 350, 160);
	GUI_DispStringHCenterAt("16 - 17 :", 350, 170);
	GUI_DispStringHCenterAt("17 - 18 :", 350, 180);
	GUI_DispStringHCenterAt("18 - 19 :", 350, 190);
	GUI_DispStringHCenterAt("19 - 20 :", 350, 200);
	GUI_DispStringHCenterAt("20 - 21 :", 350, 210);
	GUI_DispStringHCenterAt("21 - 22 :", 350, 220);
	GUI_DispStringHCenterAt("22 - 23 :", 350, 230);
	GUI_DispStringHCenterAt("23 - 00 :", 350, 240);


	GUI_MEMDEV_CopyToLCD(hMem);
	GUI_MEMDEV_Delete(hMem);
}

void draw_clock(void)
{
	int date_tmp[3];
	char date[12];

	hMem_clock = GUI_MEMDEV_Create(46,46,180,180);
	GUI_MEMDEV_Select(hMem_clock);
	GUI_Clear();

	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt("SEIKO", 136, 80);

	// ---------- SAT ---------- //
	angle = 30 * (get_time(HOURS_TYPE) % 12) + (get_time(MINUTES_TYPE)/2) + (get_time(SECONDS_TYPE)/30);
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*50));
	y_coordinate=(136-(cos(angle)*50));

	GUI_SetColor(GUI_BLACK);
	GUI_SetPenSize( 10 );
	GUI_DrawLine(136, 136, x_coordinate, y_coordinate);//sat


	// ---------- MINUT ---------- //
	angle = 6 * (get_time(MINUTES_TYPE)) + (get_time(SECONDS_TYPE)/10);
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*80));
	y_coordinate=(136-(cos(angle)*80));

	GUI_SetPenSize( 6 );
	GUI_DrawLine(136, 136, x_coordinate, y_coordinate);//minut


	// ---------- SEKUND ---------- //
	angle = 6 * (get_time(SECONDS_TYPE));
	angle=(angle/57.29577951) ; //Convert degrees to radians
	x_coordinate=(136+(sin(angle)*90));
	y_coordinate=(136-(cos(angle)*90));

	GUI_SetColor(GUI_RED);
	GUI_SetPenSize( 4 );
	GUI_DrawLine(136, 136, x_coordinate, y_coordinate);//sekund


	// ---------- DATUM ---------- //
	GUI_SetColor(GUI_BLACK);
	date_tmp[0] = get_date(DATE_TYPE);
	date_tmp[1] = get_date(MONTH_TYPE);
	date_tmp[2] = get_date(YEAR_TYPE);
	sprintf((char*)date, "%02d:%02d:20%02d\r", date_tmp[0], date_tmp[1], date_tmp[2]);
	GUI_SetFont(&GUI_Font24B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_DispStringHCenterAt(date, 136, 200);


	GUI_MEMDEV_CopyToLCD(hMem_clock);
	GUI_MEMDEV_Delete(hMem_clock);
}

void draw_temperature(void)
{
	char temperatura_string[10];
	int tmp;

	hMem_temp = GUI_MEMDEV_Create(375, 0, 80, 255);
	GUI_MEMDEV_Select(hMem_temp);
	GUI_SetColor(GUI_BLACK);

	if( (get_time(MINUTES_TYPE) == 0) && (get_time(SECONDS_TYPE) == 0) )
	{
		if(get_time(HOURS_TYPE) == 1)
			GUI_Clear();
		GUI_SetColor(GUI_BLACK);
		tmp = get_temp();
		sprintf( (char *)temperatura_string, "%02d C\r", tmp);
		GUI_SetFont(&GUI_Font13B_ASCII);
		GUI_SetTextMode(GUI_TM_TRANS);
		if(get_time(HOURS_TYPE) == 0)
			GUI_DispStringHCenterAt(temperatura_string, 390, 240);
		else
			GUI_DispStringHCenterAt(temperatura_string, 390, 10*get_time(HOURS_TYPE));
		GUI_Delay(1000);
	}


	GUI_MEMDEV_CopyToLCD(hMem_temp);
	GUI_MEMDEV_Delete(hMem_temp);
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
