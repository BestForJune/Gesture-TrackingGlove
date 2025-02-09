/*
Library:					SPI LCD - ILI9341
Written by:				Mohamed Yaqoob (MYaqoobEmbedded YouTube Channel)
Date Written:			09/12/2018
Last modified:		-/-
Description:			This is an STM32 device driver library for the ILI9341 SPI LCD display, using STM HAL libraries
										
* Copyright (C) 2018 - M. Yaqoob
   This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
   of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
	
   This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
   or indirectly by this software, read more about this on the GNU General Public License.
*/

//Header files
#include "MY_ILI9341.h"

uint16_t black_line[ILI9341_WIDTH*2] = {0x0000};
static uint8_t rotationNum=1;
static bool _cp437    = false;
static SPI_HandleTypeDef lcdSPIhandle;
//Chip Select pin
static GPIO_TypeDef  *tftCS_GPIO;
static uint16_t tftCS_PIN;
//Data Command pin
static GPIO_TypeDef  *tftDC_GPIO;
static uint16_t tftDC_PIN;
//Reset pin
static GPIO_TypeDef  *tftRESET_GPIO;
static uint16_t tftRESET_PIN;

static //Text simple font array (You can your own font)
const unsigned char font1[] = {
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x22, 0x54, 0x54, 0x78, 0x42,
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0x7D, 0x12, 0x11, 0x12, 0x7D,
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x3A, 0x44, 0x44, 0x44, 0x3A, 
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x3D, 0x42, 0x42, 0x42, 0x3D, 
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0x55, 0x00, 0x55, 0x00, 0x55, 
	0xAA, 0x55, 0xAA, 0x55, 0xAA, 
	0xFF, 0x55, 0xFF, 0x55, 0xFF, 
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0xFC, 0x4A, 0x4A, 0x4A, 0x34, 
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00  
};

//***** Functions prototypes *****//
//1. Write Command to LCD
void ILI9341_SendCommand(uint8_t com)
{
	//*(__IO uint8_t *)(0x60000000) = com;
	uint8_t tmpCmd = com;
	//Set DC HIGH for COMMAND mode
	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_RESET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);
	//Write byte using SPI
	HAL_SPI_Transmit(&lcdSPIhandle, &tmpCmd, 1, 5);
	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}

//2. Write data to LCD
void ILI9341_SendData(uint8_t data)
{
	//*(__IO uint8_t *)(0x60040000) = data;
	uint8_t tmpCmd = data;
	//Set DC LOW for DATA mode
	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);
	//Write byte using SPI
	HAL_SPI_Transmit(&lcdSPIhandle, &tmpCmd, 1, 5);
	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}
//2.2 Write multiple/DMA
void ILI9341_SendData_Multi(uint16_t Colordata, uint32_t size)
{
	uint8_t colorL,colorH;
	
	//Set DC LOW for DATA mode
	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);
	//Write byte using SPI
	HAL_SPI_Transmit(&lcdSPIhandle, (uint8_t *)&Colordata, size, 1000);
	//Wait for end of DMA transfer
	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}

//3. Set cursor position
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {

  ILI9341_SendCommand (ILI9341_COLUMN_ADDR);
  ILI9341_SendData(x1>>8);
  ILI9341_SendData(x1 & 0xFF);
  ILI9341_SendData(x2>>8);
  ILI9341_SendData(x2 & 0xFF);

  ILI9341_SendCommand (ILI9341_PAGE_ADDR);
  ILI9341_SendData(y1>>8);
  ILI9341_SendData(y1 & 0xFF);
  ILI9341_SendData(y2>>8);
  ILI9341_SendData(y2 & 0xFF);
  ILI9341_SendCommand (ILI9341_GRAM);
}
//4. Initialise function
void ILI9341_Init(SPI_HandleTypeDef *spiLcdHandle, GPIO_TypeDef *csPORT, uint16_t csPIN, GPIO_TypeDef *dcPORT, uint16_t dcPIN, GPIO_TypeDef *resetPORT, uint16_t resetPIN)
 {
	 //Copy SPI settings
	 memcpy(&lcdSPIhandle, spiLcdHandle, sizeof(*spiLcdHandle));
	 //CS pin
	 tftCS_GPIO = csPORT;
	 tftCS_PIN = csPIN;
	 //DC pin
	 tftDC_GPIO = dcPORT;
	 tftDC_PIN = dcPIN;
	 HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
	 //RESET pin
	 tftRESET_GPIO = resetPORT;
	 tftRESET_PIN = resetPIN;
	 HAL_GPIO_WritePin(resetPORT, resetPIN, GPIO_PIN_SET);  //Turn LCD ON
	 
   ILI9341_SendCommand (ILI9341_RESET); // software reset comand
   HAL_Delay(100);
   ILI9341_SendCommand (ILI9341_DISPLAY_OFF); // display off
   //------------power control------------------------------
   ILI9341_SendCommand (ILI9341_POWER1); // power control
   ILI9341_SendData   (0x26); // GVDD = 4.75v
   ILI9341_SendCommand (ILI9341_POWER2); // power control
   ILI9341_SendData   (0x11); // AVDD=VCIx2, VGH=VCIx7, VGL=-VCIx3
   //--------------VCOM-------------------------------------
   ILI9341_SendCommand (ILI9341_VCOM1); // vcom control
   ILI9341_SendData   (0x35); // Set the VCOMH voltage (0x35 = 4.025v)
   ILI9341_SendData   (0x3e); // Set the VCOML voltage (0x3E = -0.950v)
   ILI9341_SendCommand (ILI9341_VCOM2); // vcom control
   ILI9341_SendData   (0xbe);

   //------------memory access control------------------------
   ILI9341_SendCommand (ILI9341_MAC); // memory access control
   ILI9341_SendData(0x48);

   ILI9341_SendCommand (ILI9341_PIXEL_FORMAT); // pixel format set
   ILI9341_SendData   (0x55); // 16bit /pixel

	 ILI9341_SendCommand(ILI9341_FRC);
   ILI9341_SendData(0);
   ILI9341_SendData(0x1F);
   //-------------ddram ----------------------------
   ILI9341_SendCommand (ILI9341_COLUMN_ADDR); // column set
   ILI9341_SendData   (0x00); // x0_HIGH---0
   ILI9341_SendData   (0x00); // x0_LOW----0
   ILI9341_SendData   (0x00); // x1_HIGH---240
   ILI9341_SendData   (0xEF); // x1_LOW----240
   ILI9341_SendCommand (ILI9341_PAGE_ADDR); // page address set
   ILI9341_SendData   (0x00); // y0_HIGH---0
   ILI9341_SendData   (0x00); // y0_LOW----0
   ILI9341_SendData   (0x01); // y1_HIGH---320
   ILI9341_SendData   (0x3F); // y1_LOW----320

   ILI9341_SendCommand (ILI9341_TEARING_OFF); // tearing effect off
   //LCD_write_cmd(ILI9341_TEARING_ON); // tearing effect on
   //LCD_write_cmd(ILI9341_DISPLAY_INVERSION); // display inversion
   ILI9341_SendCommand (ILI9341_Entry_Mode_Set); // entry mode set
   // Deep Standby Mode: OFF
   // Set the output level of gate driver G1-G320: Normal display
   // Low voltage detection: Disable
   ILI9341_SendData   (0x07);
   //-----------------display------------------------
   ILI9341_SendCommand (ILI9341_DFC); // display function control
   //Set the scan mode in non-display area
   //Determine source/VCOM output in a non-display area in the partial display mode
   ILI9341_SendData   (0x0a);
   //Select whether the liquid crystal type is normally white type or normally black type
   //Sets the direction of scan by the gate driver in the range determined by SCN and NL
   //Select the shift direction of outputs from the source driver
   //Sets the gate driver pin arrangement in combination with the GS bit to select the optimal scan mode for the module
   //Specify the scan cycle interval of gate driver in non-display area when PTG to select interval scan
   ILI9341_SendData   (0x82);
   // Sets the number of lines to drive the LCD at an interval of 8 lines
   ILI9341_SendData   (0x27);
   ILI9341_SendData   (0x00); // clock divisor

   ILI9341_SendCommand (ILI9341_SLEEP_OUT); // sleep out
   HAL_Delay(100);
   ILI9341_SendCommand (ILI9341_DISPLAY_ON); // display on
   HAL_Delay(100);
   ILI9341_SendCommand (ILI9341_GRAM); // memory write
   HAL_Delay(5);
 }

//5. Write data to a single pixel
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    ILI9341_SetCursorPosition(x, y, x, y);
	ILI9341_SendData(color>>8);
	ILI9341_SendData(color&0xFF);
}

//6. Fill the entire screen with a background color
void ILI9341_Fill(uint8_t* screen) {
	ILI9341_SetCursorPosition(0, 0,   ILI9341_WIDTH -1, ILI9341_HEIGHT -1);
	//Write byte using SPI

	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);

	for(int lcv = 0; lcv < ILI9341_HEIGHT; lcv++){
		HAL_SPI_Transmit(&lcdSPIhandle, screen, ILI9341_WIDTH * 2, 2000);
	}

	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}

void ILI9341_Fill_Line (uint8_t* each_line, uint16_t index) {
	ILI9341_SetCursorPosition(0, index,   ILI9341_WIDTH -1, ILI9341_HEIGHT-1);
	//Write byte using SPI

	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);

	uint16_t orig_line[ILI9341_WIDTH*2] = {0x0000};
	if (each_line[0] == 1){
		for(int lcv = 0; lcv < 25; lcv ++) {
			orig_line[15+lcv] = COLOR_RED;
		}
	}
	if (each_line[1] == 1){
		for(int lcv = 0; lcv < 25; lcv ++) {
			orig_line[50+lcv] = COLOR_PURPLE;
		}
	}
	if (each_line[2] == 1){
		for(int lcv = 0; lcv < 25; lcv ++) {
			orig_line[85+lcv] = COLOR_YELLOW;
		}
	}
	if (each_line[3] == 1){
		for(int lcv = 0; lcv < 25; lcv ++) {
			orig_line[120+lcv] = COLOR_NAVY;
		}
	}
	if (each_line[4] == 1){
		for(int lcv = 0; lcv < 25; lcv ++) {
			orig_line[155+lcv] = COLOR_WHITE;
		}
	}

	HAL_SPI_Transmit(&lcdSPIhandle, orig_line, ILI9341_WIDTH*2, 2000);

	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}

void ILI9341_Fill_Black_Line(uint16_t index) {
	ILI9341_SetCursorPosition(0, index,   ILI9341_WIDTH -1, ILI9341_HEIGHT-1);
	//Write byte using SPI

	HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
	//Put CS LOW
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);

	uint16_t orig_line[ILI9341_WIDTH*2] = {0x0000};
	HAL_SPI_Transmit(&lcdSPIhandle, orig_line, ILI9341_WIDTH*2, 2000);

	//Bring CS HIGH
	HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
}


//7. Rectangle drawing functions
void ILI9341_Fill_Rect(unsigned int x0,unsigned int y0, unsigned int x1,unsigned int y1, uint16_t color) { 
	uint32_t n = ((x1+1)-x0)*((y1+1)-y0);
	if (n>ILI9341_PIXEL_COUNT) n=ILI9341_PIXEL_COUNT;
	ILI9341_SetCursorPosition(x0, y0, x1, y1);
	while (n) {
      n--;
      ILI9341_SendData(color>>8);
	  ILI9341_SendData(color&0xff);
	}

}

//8. Circle drawing functions
void ILI9341_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  ILI9341_DrawPixel(x0  , y0+r, color);
  ILI9341_DrawPixel(x0  , y0-r, color);
  ILI9341_DrawPixel(x0+r, y0  , color);
  ILI9341_DrawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    ILI9341_DrawPixel(x0 + x, y0 + y, color);
    ILI9341_DrawPixel(x0 - x, y0 + y, color);
    ILI9341_DrawPixel(x0 + x, y0 - y, color);
    ILI9341_DrawPixel(x0 - x, y0 - y, color);
    ILI9341_DrawPixel(x0 + y, y0 + x, color);
    ILI9341_DrawPixel(x0 - y, y0 + x, color);
    ILI9341_DrawPixel(x0 + y, y0 - x, color);
    ILI9341_DrawPixel(x0 - y, y0 - x, color);
  }
}
static void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
	int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      ILI9341_DrawPixel(x0 + x, y0 + y, color);
      ILI9341_DrawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      ILI9341_DrawPixel(x0 + x, y0 - y, color);
      ILI9341_DrawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      ILI9341_DrawPixel(x0 - y, y0 + x, color);
      ILI9341_DrawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      ILI9341_DrawPixel(x0 - y, y0 - x, color);
      ILI9341_DrawPixel(x0 - x, y0 - y, color);
    }
  }
}
static void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      ILI9341_drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      ILI9341_drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      ILI9341_drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      ILI9341_drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}
void ILI9341_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	ILI9341_drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

//9. Line drawing functions
void ILI9341_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      ILI9341_DrawPixel(y0, x0, color);
    } else {
      ILI9341_DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}	

void ILI9341_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	ILI9341_drawLine(x, y, x+w-1, y, color);
}

void ILI9341_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	ILI9341_drawLine(x, y, x, y+h-1, color);
}
//10. Triangle drawing
void ILI9341_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	ILI9341_drawLine(x0, y0, x1, y1, color);
  ILI9341_drawLine(x1, y1, x2, y2, color);
  ILI9341_drawLine(x2, y2, x0, y0, color);
}
void ILI9341_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    ILI9341_drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b) swap(a,b);
    ILI9341_drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b) swap(a,b);
    ILI9341_drawFastHLine(a, y, b-a+1, color);
	}
}

//11. Text printing functions
void ILI9341_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	if(rotationNum == 1 || rotationNum ==3)
	{
		if((x >= ILI9341_WIDTH)            || // Clip right
     (y >= ILI9341_HEIGHT)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;
	}
	else
	{
		if((y >= ILI9341_WIDTH)            || // Clip right
     (x >= ILI9341_HEIGHT)           || // Clip bottom
     ((y + 6 * size - 1) < 0) || // Clip left
     ((x + 8 * size - 1) < 0))   // Clip top
    return;
	}


  if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = pgm_read_byte(font1+(c*5)+i);
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          ILI9341_DrawPixel(x+i, y+j, color);
        else {  // big size
          ILI9341_Fill_Rect(x+(i*size), y+(j*size), size + x+(i*size), size+1 + y+(j*size), color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          ILI9341_DrawPixel(x+i, y+j, bg);
        else {  // big size
          ILI9341_Fill_Rect(x+i*size, y+j*size, size + x+i*size, size+1 + y+j*size, bg);
        }
      }
      line >>= 1;
    }
  }
}
void ILI9341_printText(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size)
{
	int16_t offset;
	offset = size*6;
	for(uint16_t i=0; i<40 && text[i]!=NULL; i++)
	{
		ILI9341_drawChar(x+(offset*i), y, text[i],color,bg,size);
	}
}

void ILI9341_printGameScore(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size)
{
	int16_t offset;
	offset = size*6;

	for(uint16_t index=0; index<40 && text[index]!=NULL; index++)
	{
//	  ILI9341_drawChar(x+(offset*index), y, text[index],color,bg,size);
	  if(!_cp437 && (text[index] >= 176)) text[index]++; // Handle 'classic' charset behavior

	  for (int8_t i=0; i<6; i++ ) {
		uint8_t line;
		if (i == 5)
		  line = 0x0;
		else
		  line = pgm_read_byte(font1+(text[index]*5)+i);
		for (int8_t j = 0; j<8; j++) {
		  if (line & 0x1) {
			  ILI9341_SetCursorPosition(x+(offset*index)+(i*size), y+(j*size), size +x+(offset*index)+(i*size), size+1 + y+(j*size));
			  //Set DC LOW for DATA mode
			  HAL_GPIO_WritePin(tftDC_GPIO, tftDC_PIN, GPIO_PIN_SET);
			  //Put CS LOW
			  HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_RESET);

			  HAL_SPI_Transmit(&lcdSPIhandle, color, 1, 10);
			  HAL_SPI_Transmit(&lcdSPIhandle, color, 1, 10);
			  //Bring CS HIGH
			  HAL_GPIO_WritePin(tftCS_GPIO, tftCS_PIN, GPIO_PIN_SET);
			}
		  line >>= 1;
		}
	  }
	}
}

//12. Image print (RGB 565, 2 bytes per pixel)
void ILI9341_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data, uint32_t size)
{
	uint32_t n = size;
	ILI9341_SetCursorPosition(x, y, w+x-1, h+y-1);
	for(uint32_t i=0; i<n ; i++)
	{
		ILI9341_SendData(data[i]);
	}
}

//13. Set screen rotation
void ILI9341_setRotation(uint8_t rotate)
{
	switch(rotate)
	{
		case 1:
			rotationNum = 1;
			ILI9341_SendCommand(ILI9341_MEMCONTROL);
			ILI9341_SendData(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;
		case 2:
			rotationNum = 2;
			ILI9341_SendCommand(ILI9341_MEMCONTROL);
			ILI9341_SendData(ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
			break;
		case 3:
			rotationNum = 3;
			ILI9341_SendCommand(ILI9341_MEMCONTROL);
			ILI9341_SendData(ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
			break;
		case 4:
			rotationNum = 4;
			ILI9341_SendCommand(ILI9341_MEMCONTROL);
			ILI9341_SendData(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
			break;
		default:
			rotationNum = 1;
			ILI9341_SendCommand(ILI9341_MEMCONTROL);
			ILI9341_SendData(ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			break;
	}
}
//14. Get screen rotation
uint8_t ILI9341_getRotation(void)
{
	return rotationNum;
}
