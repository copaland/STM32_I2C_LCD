/*
 * liquidcrystal_i2c.h
 *
 *  Created on: Feb 5, 2022
 *      Author: Nicky Kim
 *  Copyright (c) 2022 KiMSON All rights reserved.
 *
 * STM32 HAL library for LCD display based on 16x2 CLCD with PCF8574
 * Currently, the "stm32f4xx_hal.h" library is used.
 * You must include and use the appropriate library for your board.
 *
 */

#ifndef INC_LIQUIDCRYSTAL_I2C_H_
#define INC_LIQUIDCRYSTAL_I2C_H_

#include "stm32f4xx_hal.h"
//#include "stm32l0xx_hal.h"
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t CurrentX;
	uint8_t CurrentY;
} LCD_Options;

static LCD_Options LCD_Opt;

//===============================================
/* Device I2C Address */
#define LCD_ADDR     		(0x27 << 1) //(0x3F << 1)

#define _LCD_USE_FREERTOS 	0 //1
#define _LCD_COLS         	16
#define _LCD_ROWS         	2
//===============================================

/* Command */
#define LCD_CLEARDISPLAY 	((uint8_t)0x01U)
#define LCD_RETURNHOME 		((uint8_t)0x02U)
#define LCD_ENTRYMODESET 	((uint8_t)0x04U)
#define LCD_DISPLAYCONTROL 	((uint8_t)0x08U)
#define LCD_CURSORSHIFT 	((uint8_t)0x10U)
#define LCD_FUNCTIONSET 	((uint8_t)0x20U)
#define LCD_SETCGRAMADDR 	((uint8_t)0x40U)
#define LCD_SETDDRAMADDR 	((uint8_t)0x80U)

/* Entry Mode */
#define LCD_ENTRYRIGHT 		((uint8_t)0x00U)
#define LCD_ENTRYLEFT 		((uint8_t)0x02U)
#define LCD_ENTRYSHIFTINC	((uint8_t)0x01U)
#define LCD_ENTRYSHIFTDEC	((uint8_t)0x00U)

/* Display On/Off */
#define LCD_DISPLAYON 		((uint8_t)0x04U)
#define LCD_DISPLAYOFF 		((uint8_t)0x00U)
#define LCD_CURSORON 		((uint8_t)0x02U)
#define LCD_CURSOROFF 		((uint8_t)0x00U)
#define LCD_BLINKON 		((uint8_t)0x01U)
#define LCD_BLINKOFF 		((uint8_t)0x00U)

/* Cursor Shift */
#define LCD_DISPLAYMOVE 	((uint8_t)0x08U)
#define LCD_CURSORMOVE 		((uint8_t)0x00U)
#define LCD_MOVERIGHT 		((uint8_t)0x04U)
#define LCD_MOVELEFT 		((uint8_t)0x00U)

/* Function Set */
#define LCD_8BITMODE 		((uint8_t)0x10U)
#define LCD_4BITMODE 		((uint8_t)0x00U)
#define LCD_2LINE 			((uint8_t)0x08U)
#define LCD_1LINE 			((uint8_t)0x00U)
#define LCD_5x10DOTS 		((uint8_t)0x04U)
#define LCD_5x8DOTS 		((uint8_t)0x00U)

/* Register Select Bit */
#define PIN_RS    			((uint8_t)0x01U) //(1 << 0)

/* Enable Bit */
#define PIN_EN    			((uint8_t)0x04U) //(1 << 2)

/* Backlight */
#define BACKLIGHT 		((uint8_t)0x08U) //(1 << 3)

void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Init();
void LCD_Clear();
void LCD_Home();
void LCD_DisplayOff();
void LCD_DisplayOn();
void LCD_BlinkOff();
void LCD_BlinkOn();
void LCD_CursorOff();
void LCD_CursorOn();
void LCD_ScrollLeft();
void LCD_ScrollRight();
void LCD_LeftToRight();
void LCD_RightToLeft();

void LCD_AutoScroll();
void LCD_NoAutoScroll();
void LCD_CreateSpecialChar(uint8_t, uint8_t[]);
void LCD_PrintSpecialChar(uint8_t);
void LCD_SetCursor(uint8_t, uint8_t);

void LCD_LoadCustomCharacter(uint8_t char_num, uint8_t *rows);
void LCD_Print(const char[]);
void LCD_Puts(uint8_t x, uint8_t y, char* str);

void LCD_Delay_us(uint16_t us);
void LCD_Delay_ms(uint8_t ms);

#endif /* INC_LIQUIDCRYSTAL_I2C_H_ */
