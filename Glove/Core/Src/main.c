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
#include "fatfs.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "audioI2S.h"
#include "MY_CS43L22.h"
#include "wav_player.h"
#include "game_arr.h"
#include "score_board.h"
#include "debounce.h"
#include "sb_input.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
enum state_type{MENU, GAME, SCOREBOARD, ENTER_NAME, END_GAME};
enum music_status_enum {PLAYING, STOP};
enum menu_option_enum {NO_USB, MENU_SB, START_GAME, MENU_SET_UP};
#define WAV_FILE1 "cello.wav"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_TIM7_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void music();
void arr_update(game_arr * arr);
void print_board(scoreboard * input_sb);
void menu_page_setup(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t screen[ILI9341_WIDTH *2];
game_arr g_arr;

int start_cycle_x = 25;
int start_cycle_y = 25;

int game_setup = 0; //if the game has been set up or not
enum state_type state = MENU;
enum music_status_enum music_status = STOP;
enum menu_option_enum menu_option = MENU_SET_UP;
enum menu_option_enum menu_option_next = NO_USB;
bool isMounted = 0;

short game_score = 0;
char buffer [3];

extern ApplicationTypeDef Appli_state;
scoreboard board;
char buf[20];
bool sb_set_up = false; //scoreboard setup

uint8_t history_index[HISTORY_NUM];
uint8_t history_thumb[HISTORY_NUM];
uint8_t history_middle[HISTORY_NUM];
uint8_t history_ring[HISTORY_NUM];
uint8_t history_little[HISTORY_NUM];
uint8_t bent_ref[HISTORY_NUM];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  for(int lcv = 0; lcv < ILI9341_WIDTH *2; lcv ++) {
  		screen[lcv] = 0;
  	}
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_FATFS_Init();
  MX_USB_HOST_Init();

  ILI9341_Init(&hspi1, LCD_CS_GPIO_Port, LCD_CS_Pin,LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin);
  ILI9341_setRotation(0);

  CS43_Init(hi2c1, MODE_I2S);
  CS43_SetVolume(230);//0-255
  CS43_Enable_RightLeft(CS43_RIGHT_LEFT);
  /* USER CODE END 1 */

//  /* MCU Configuration--------------------------------------------------------*/
//
//  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//  HAL_Init();
//
//  /* USER CODE BEGIN Init */
//  for(int lcv = 0; lcv < ILI9341_WIDTH *2; lcv ++) {
//  		screen[lcv] = 0;
//  	}
//  /* USER CODE END Init */
//
//  /* Configure the system clock */
//  SystemClock_Config();
//
//  /* USER CODE BEGIN SysInit */
//
//  /* USER CODE END SysInit */
//
//  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_DMA_Init();
//  MX_SPI1_Init();
//  MX_TIM2_Init();
//  MX_I2C1_Init();
//  MX_I2S3_Init();
//  MX_FATFS_Init();
//  MX_USB_HOST_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  menu_page_setup();
  scoreboard_init(&board);
  audioI2S_setHandle(&hi2s3);
  init_debounce(history_index, bent_ref);
  init_debounce(history_thumb, bent_ref);
  init_debounce(history_middle, bent_ref);
  init_debounce(history_ring, bent_ref);
  init_debounce(history_little, bent_ref);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */

	  if(state == MENU) {
		  update_debounce(history_index, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1));
		  update_debounce(history_thumb, HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0));
		  if(menu_option == NO_USB) {
			  if(isMounted) { //state transition
				  menu_option = MENU_SET_UP;
				  menu_option_next = START_GAME;
			  }
		  }
		  else if(menu_option == START_GAME) {
			  if(isMounted == 0) { //state transition
				  menu_option = MENU_SET_UP;
				  menu_option_next = NO_USB;
			  }
			  else if(match_bent(history_thumb, bent_ref)) {
				  state = GAME;
				  game_setup = 0;
			  } else if(match_bent(history_index, bent_ref)) { //state transition
				  menu_option = MENU_SET_UP;
				  menu_option_next = MENU_SB;
			  }
		  }
		  else if(menu_option == MENU_SB) {
			  if(isMounted == 0) { //state transition
				  menu_option = MENU_SET_UP;
				  menu_option_next = NO_USB;
			  }
			  else if(match_bent(history_thumb, bent_ref)) {
				  state = SCOREBOARD;
			  } else if(match_bent(history_index, bent_ref)) { //state transition
				  menu_option = MENU_SET_UP;
				  menu_option_next = START_GAME;
			  }
		  }
		  menu_page_setup();
	  }
	  //test button and changes on screen
	  else if (state == GAME){
		  if(!game_setup) {
			  ILI9341_Fill(screen);
			  HAL_TIM_Base_Start_IT(&htim7);
			  game_setup = 1;
		  }
		  int ptr = g_arr.head;
		  while(ptr != MAX_LINE) {
//		          arr->lines[ptr].y_pos++;
		          ILI9341_Fill_Black_Line(g_arr.lines[ptr].y_pos - 10);
		          ILI9341_Fill_Line(&g_arr.lines[ptr].pattern, g_arr.lines[ptr].y_pos + 10);
		          ptr = g_arr.lines[ptr].next;
		  }
		  sprintf(buffer, "%d", game_score);
		  ILI9341_printGameScore(buffer, 190, 20, COLOR_GREEN, COLOR_RED, 3);
//		  test_track++;
		  //test the situation when game ends
		  //should be deleted
		  if (music_status == STOP){
			HAL_TIM_Base_Stop_IT(&htim7);
			ILI9341_Fill(screen);
			state = ENTER_NAME;
//			test_track = 0;
			game_setup = 0;
		  }
	  }
	  else if( state == ENTER_NAME) {
		  game_end(game_score);
		  game_score = 0;
		  state = END_GAME;
		  ILI9341_Fill(screen);
	  }
	  else if(state == END_GAME) {
		  state = MENU;
		  menu_option = MENU_SET_UP;
	  }
	  else if (state == SCOREBOARD){
		  if(!sb_set_up) {
			ILI9341_Fill(screen);
			ILI9341_printText("Scoreboard", 50, 20, COLOR_GREEN, COLOR_GREEN, 2);
			print_board(&board);
			sb_set_up = true;
		  }
//		scoreboard_update(&board, "P1", 2, game_score);
		if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)) {
			state = MENU;
			menu_option = MENU_SET_UP;
			menu_option_next = MENU_SB;
			sb_set_up = false;
		}
	  }

	  MX_USB_HOST_Process();
	  music();

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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 271;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 6;
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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 25599;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 599;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 1000 - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000 - 1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|LCD_DC_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 LCD_DC_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|LCD_DC_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : user_button_Pin */
  GPIO_InitStruct.Pin = user_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(user_button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15
                           PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3
                           PD5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  /* Prevent unused argument(s) compilation warning */
//  if(htim->Instance == TIM2){
//	  for (int i = 0; i < 15; i++){
//		  ILI9341_fillCircle(start_cycle_x,start_cycle_y + 15 * tim2_i, 10, COLOR_RED);
//		  ILI9341_fillCircle(start_cycle_x,start_cycle_y + 15 * tim2_i, 10, COLOR_BLACK);
////	  tim2_i++;
////	  if (tim2_i == 14) {HAL_TIM_Base_Stop(&htim2);}
//	  }
//	  HAL_TIM_Base_Stop(&htim2);
//  }
//
//  /* NOTE : This function should not be modified, when the callback is needed,
//            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
//   */
//}


void menu_page_setup(void){
	//menu page setup
//	ILI9341_Fill(screen);
//    ILI9341_printText("GLOVE", 50, 100, COLOR_GREEN, COLOR_GREEN, 5);
    if(menu_option == MENU_SET_UP){
    	ILI9341_Fill(screen);
		ILI9341_printText("GLOVE", 50, 100, COLOR_GREEN, COLOR_GREEN, 5);
		menu_option = menu_option_next;
    }
    else if(menu_option == NO_USB) {
    	ILI9341_printText("Please", 70, 180, COLOR_GREEN, COLOR_RED, 2);
    	ILI9341_printText("Insert USB", 50, 220, COLOR_GREEN, COLOR_RED, 2);
    }
    else if(menu_option == START_GAME) {
    	ILI9341_printText("Start Game", 50, 180, COLOR_GREEN, COLOR_RED, 2);
    	ILI9341_printText("ScoreBoard", 50, 220, COLOR_GREEN, COLOR_BLACK, 2);
//		HAL_Delay(300);
    }
    else if(menu_option == MENU_SB) {
    	ILI9341_printText("Start Game", 50, 180, COLOR_GREEN, COLOR_BLACK, 2);
		ILI9341_printText("ScoreBoard", 50, 220, COLOR_GREEN, COLOR_RED, 2);
//		HAL_Delay(300);
    }

    return;
}


void music() {
		if(Appli_state == APPLICATION_START) {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		}
		else if(Appli_state == APPLICATION_DISCONNECT) {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		  f_mount(NULL, (TCHAR const*)"", 0);
		  isMounted = 0;
		}

		if(Appli_state == APPLICATION_READY) {
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			if(!isMounted){
				f_mount(&USBHFatFS, (const TCHAR*) USBHPath, 0);
				isMounted = 1;
			}

			if(state == GAME) {
				if(music_status == STOP){
					uint8_t seed;

					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
					wavPlayer_fileSelect(WAV_FILE1, &seed);
					game_arr_init(&g_arr, seed);
					wavPlayer_play();
					music_status = PLAYING;
				} else {
					if(wavPlayer_isFinished()) {
//						state = MENU;
						music_status = STOP;
						wavPlayer_stop();
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
					} else {
						wavPlayer_process();
					}
				}
			}
		}
}

void arr_update(game_arr * arr) {
	int un_match = 0;
    //condition to generate a new line
    if(arr->lines[arr->head].y_pos >= 90) {
        new_line(arr);
    }
    uint8_t ptr = arr->head; // linked list traverse pointer
    while(ptr != MAX_LINE) {
        arr->lines[ptr].y_pos++;
//        ILI9341_Fill_Black_Line(arr->lines[ptr].y_pos - 10);
//        ILI9341_Fill_Line(&arr->lines[ptr], arr->lines[ptr].y_pos + 10);
        un_match = 0;
		if(HEIGHT > arr->lines[ptr].y_pos && arr->lines[ptr].y_pos > 270){
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) ^ arr->lines[ptr].pattern[0]){
				un_match = 1;
//				arr->lines[ptr].pattern[0] = 0x0;
			}
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) ^ arr->lines[ptr].pattern[1]){
				un_match = 1;
//				arr->lines[ptr].pattern[1] = 0x0;
			}
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) ^ arr->lines[ptr].pattern[2]){
				un_match = 1;
//				arr->lines[ptr].pattern[2] = 0x0;
			}
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) ^ arr->lines[ptr].pattern[3]){
				un_match = 1;
//				arr->lines[ptr].pattern[3] = 0x0;
			}
			int test1 = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5);
			int test2 =  arr->lines[ptr].pattern[4];
			if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5) ^ arr->lines[ptr].pattern[4]){
				un_match = 1;
//				arr->lines[ptr].pattern[4] = 0x0;
			}
			
			if (!un_match){
				if(arr->lines[ptr].score) {
					game_score++;
					arr->lines[ptr].pattern[0] = 0x0;
					arr->lines[ptr].pattern[1] = 0x0;
					arr->lines[ptr].pattern[2] = 0x0;
					arr->lines[ptr].pattern[3] = 0x0;
					arr->lines[ptr].pattern[4] = 0x0;
					arr->lines[ptr].score = 0;
				}
				arr->lines[ptr].next = MAX_LINE;
			}
		}
        if(arr->lines[arr->lines[ptr].next].y_pos >= HEIGHT) {
            arr->lines[ptr].next = MAX_LINE;
        }

        ptr = arr->lines[ptr].next;
    }
}

void print_board(scoreboard * input_sb) {
    for(short lcv = 0; lcv < MAX_PLAYER; lcv++) {
        to_string(&(input_sb->players[lcv]), input_sb->buf);
        ILI9341_printText(input_sb->buf, 50, 80 + lcv * 20, COLOR_GREEN, COLOR_BLACK, 2);
    }
    ILI9341_printText("Bent thumb to go back", 50, 300, COLOR_GREEN, COLOR_BLACK, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback and toggle LED
  if (htim == &htim7 )
  {
	  arr_update(&g_arr);
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_11);
  }
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
