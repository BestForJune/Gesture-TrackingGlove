/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t circle[] = {1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1};
uint8_t rectan[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0};
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init(&hspi1, LCD_CS_GPIO_Port, LCD_CS_Pin,LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin);
  ILI9341_setRotation(0);
  ILI9341_Fill(COLOR_BLACK);

  int startmenu_x = 50;
  int startmenu_y = 100;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  //menu page setup
	  ILI9341_printText("Start", startmenu_x, startmenu_y, COLOR_GREEN, COLOR_GREEN, 5);
	  HAL_Delay(500);
	  ILI9341_printText("Start", startmenu_x, startmenu_y, COLOR_BLACK, COLOR_BLACK, 5);

	  ILI9341_printText("ScoreBoard", 50, 180, COLOR_GREEN, COLOR_RED, 2);
	  ILI9341_printText("Music", 50, 220, COLOR_GREEN, COLOR_RED, 2);
	  ILI9341_printText("Setting", 50, 260, COLOR_GREEN, COLOR_RED, 2);

	  //test button and changes on screen
	  if ((GPIOA->IDR & GPIO_PIN_0) != (uint32_t)GPIO_PIN_RESET){
		  ILI9341_Fill(COLOR_BLACK);
		  while ((GPIOA->IDR & GPIO_PIN_0) == (uint32_t)GPIO_PIN_RESET){
  //			  ILI9341_printText("ScoreBoard", 20, 50, COLOR_GREEN, COLOR_RED, 3);
  //			  ILI9341_printText("1. 1234", 50, 90, COLOR_GREEN, COLOR_GREEN, 2);
			  GamePage();
		  }
		  ILI9341_Fill(COLOR_BLACK);

	  }
	  /* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
  }


  void GamePage (void){
	int cycle_tracker = 0;
	int cycle = circle[0];
	int start_cycle_x = 90;
	int start_cycle_y = 25;

	int recta_tracker = 0;
	int recta = rectan[0];
	int start_recta_x0 = 110;
	int start_recta_y0 = 15;
	int start_recta_x1 = 130;
	int start_recta_y1 = 35;

	for (int i = 1; i < strlen(circle); i++){
		if (circle[i]){
			if (circle[i] == 2 && circle[i-1] != 2){
				cycle_tracker = i;
			}
			else if (circle[i] == 1 && circle[i-1] != 1){
				cycle = i - cycle_tracker;
				cycle_tracker = 0;
			}
			ILI9341_fillCircle(start_cycle_x - 3 * cycle,start_cycle_y + 15 * cycle, 10, COLOR_RED);
			ILI9341_fillCircle(start_cycle_x - 3 * (cycle - cycle_tracker),start_cycle_y + 15 * (cycle-cycle_tracker), 10, COLOR_RED);
		}

		if (rectan[i]){
			if (rectan[i] == 2 && rectan[i-1] != 2){
				recta_tracker = i;
			}
			else if (rectan[i] == 1 && rectan[i-1] != 1){
				recta = i - recta_tracker;
				recta_tracker = 0;
			}
			ILI9341_Fill_Rect(start_recta_x0 - 2 * recta,start_recta_y0 + 15 * recta, start_recta_x1 - 2 * recta, start_recta_y1 + 15 * recta, COLOR_NAVY);
			ILI9341_Fill_Rect(start_recta_x0 - 2 * (recta-recta_tracker),start_recta_y0 + 15 * (recta-recta_tracker), start_recta_x1 - 2 * (recta-recta_tracker), start_recta_y1 + 15 * (recta-recta_tracker), COLOR_NAVY);
		}

//			ILI9341_fillCircle(start_cycle_x,start_cycle_y, 10, COLOR_RED);
////				HAL_Delay(200);
////				ILI9341_fillCircle(start_cycle_x,start_cycle_y, 10, COLOR_BLACK);
//			start_cycle_x -= 3;
//			start_cycle_y += 10;
//
//			ILI9341_Fill_Rect(start_recta_x0,start_recta_y0, start_recta_x1, start_recta_y1, COLOR_NAVY);
////				HAL_Delay(200);
////				ILI9341_fillCircle(start_cycle_x,start_cycle_y, 10, COLOR_BLACK);
////				ILI9341_Fill_Rect(start_recta_x0,start_recta_y0, start_recta_x1, start_recta_y1, COLOR_BLACK);
//			start_recta_x0 -= 2;
//			start_recta_y0 += 10;
//			start_recta_x1 -= 2;
//			start_recta_y1 += 10;
		HAL_Delay(200);
		//clean screen
		ILI9341_fillCircle(start_cycle_x - 3 * cycle,start_cycle_y + 15 * cycle, 10, COLOR_BLACK);
		ILI9341_fillCircle(start_cycle_x - 3 * (cycle - cycle_tracker),start_cycle_y + 15 * (cycle-cycle_tracker), 10, COLOR_BLACK);
		ILI9341_Fill_Rect(start_recta_x0 - 2 * recta,start_recta_y0 + 15 * recta, start_recta_x1 - 2 * recta, start_recta_y1 + 15 * recta, COLOR_BLACK);
		ILI9341_Fill_Rect(start_recta_x0 - 2 * (recta-recta_tracker),start_recta_y0 + 15 * (recta-recta_tracker), start_recta_x1 - 2 * (recta-recta_tracker), start_recta_y1 + 15 * (recta-recta_tracker), COLOR_BLACK);

		cycle += 1;
		recta++;
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 128;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_DC_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : user_button_Pin */
  GPIO_InitStruct.Pin = user_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(user_button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
