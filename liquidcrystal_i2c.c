/*
 * liquidcrystal_i2c.c
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

#include "liquidcrystal_i2c.h"

#if _LCD_USE_FREERTOS==1
#include "cmsis_os.h"
#endif

extern I2C_HandleTypeDef hi2c1;
/* Private variable */
extern LCD_Options LCD_Opt;

uint8_t special1[8] = {
        0b10000,
        0b11000,
        0b11100,
        0b11110,
        0b11100,
        0b11000,
        0b10000,
        0b00000
};

uint8_t special2[8] = {
        0b11000,
        0b11000,
        0b00110,
        0b01001,
        0b01000,
        0b01001,
        0b00110,
        0b00000
};


HAL_StatusTypeDef LCD_SendInternal(uint8_t lcd_addr, uint8_t data, uint8_t flags) {
    HAL_StatusTypeDef res;
    for(;;) {
        res = HAL_I2C_IsDeviceReady(&hi2c1, lcd_addr, 1, HAL_MAX_DELAY);
        if(res == HAL_OK)
            break;
    }

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
    data_arr[1] = up|flags|BACKLIGHT;
    data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
    data_arr[3] = lo|flags|BACKLIGHT;

    res = HAL_I2C_Master_Transmit(&hi2c1, lcd_addr, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
    LCD_Delay_ms(5);
    return res;
}

void LCD_SendCommand(uint8_t cmd) {
    LCD_SendInternal(LCD_ADDR, cmd, 0);
}

void LCD_SendData(uint8_t data) {
    LCD_SendInternal(LCD_ADDR, data, PIN_RS);
}

void LCD_Init(void) {
	LCD_Opt.CurrentX = 0;
	LCD_Opt.CurrentY = 0;
	LCD_Opt.DisplayFunction = LCD_8BITMODE | LCD_5x8DOTS | LCD_2LINE;
	LCD_Opt.DisplayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD_Opt.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDEC;

    // 8-bit mode, 2 lines, 5x7 format
    LCD_SendCommand(LCD_FUNCTIONSET | LCD_Opt.DisplayFunction);//0x38
    // display & cursor home
    LCD_SendCommand(LCD_RETURNHOME); //0x02
    // display on, right shift, underline off, blink off
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);//0x0c
    // clear display (optional here)
    LCD_SendCommand(LCD_CLEARDISPLAY); //0x01

    LCD_SendCommand(LCD_ENTRYMODESET | LCD_Opt.DisplayMode); //0x06

    LCD_CreateSpecialChar(0, special1);
    LCD_CreateSpecialChar(1, special2);
}

void LCD_Clear()
{
	LCD_SendCommand(LCD_CLEARDISPLAY);
	LCD_Delay_ms(2);
}

void LCD_Home()
{
	LCD_SendCommand(LCD_RETURNHOME);
	LCD_Delay_ms(2);
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row >= _LCD_ROWS)
		row = 0;
	LCD_Opt.CurrentX = col;
	LCD_Opt.CurrentY = row;
	LCD_SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_DisplayOff()
{
	LCD_Opt.DisplayControl &= ~LCD_DISPLAYON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_DisplayOn()
{
	LCD_Opt.DisplayControl |= LCD_DISPLAYON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_CursorOff()
{
	LCD_Opt.DisplayControl &= ~LCD_CURSORON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_CursorOn()
{
	LCD_Opt.DisplayControl |= LCD_CURSORON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_BlinkOff()
{
	LCD_Opt.DisplayControl &= ~LCD_BLINKON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_BlinkOn()
{
	LCD_Opt.DisplayControl |= LCD_BLINKON;
	LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_Opt.DisplayControl);
}

void LCD_ScrollLeft(void)
{
	LCD_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_ScrollRight(void)
{
	LCD_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_LeftToRight(void)
{
	LCD_Opt.DisplayMode |= LCD_ENTRYLEFT;
	LCD_SendCommand(LCD_ENTRYMODESET | LCD_Opt.DisplayMode);
}

void LCD_RightToLeft(void)
{
	LCD_Opt.DisplayMode &= ~LCD_ENTRYLEFT;
	LCD_SendCommand(LCD_ENTRYMODESET | LCD_Opt.DisplayMode);
}

void LCD_AutoScroll(void)
{
	LCD_Opt.DisplayMode |= LCD_ENTRYSHIFTINC;
	LCD_SendCommand(LCD_ENTRYMODESET | LCD_Opt.DisplayMode);
}

void LCD_NoAutoScroll(void)
{
	LCD_Opt.DisplayMode &= ~LCD_ENTRYSHIFTINC;
	LCD_SendCommand(LCD_ENTRYMODESET | LCD_Opt.DisplayMode);
}

void LCD_CreateSpecialChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7;
  LCD_SendCommand(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++)
  {
    LCD_SendData(charmap[i]);
  }
}

void LCD_PrintSpecialChar(uint8_t index)
{
  LCD_SendData(index);
}

void LCD_LoadCustomCharacter(uint8_t char_num, uint8_t *rows)
{
  LCD_CreateSpecialChar(char_num, rows);
}

void LCD_Print(const char c[])
{
  while(*c) LCD_SendData(*c++);
}

void LCD_Puts(uint8_t x, uint8_t y, char* str)
{
	LCD_SetCursor(x, y);
	while (*str)
	{
		if (LCD_Opt.CurrentX >= _LCD_COLS)
		{
			LCD_Opt.CurrentX = 0;
			LCD_Opt.CurrentY++;
			LCD_SetCursor(LCD_Opt.CurrentX, LCD_Opt.CurrentY);
		}
		if (*str == '\n')
		{
			LCD_Opt.CurrentY++;
			LCD_SetCursor(LCD_Opt.CurrentX, LCD_Opt.CurrentY);
		}
		else if (*str == '\r')
		{
			LCD_SetCursor(0, LCD_Opt.CurrentY);
		}
		else
		{
			LCD_SendData(*str);//LCD_Print(*str);
			LCD_Opt.CurrentX++;
		}
		str++;
	}
}

void LCD_Delay_us(uint16_t us)
{
  uint32_t  Div = (SysTick->LOAD+1)/1000;
  uint32_t  StartMicros = HAL_GetTick()*1000 + (1000- SysTick->VAL/Div);
  while((HAL_GetTick()*1000 + (1000-SysTick->VAL/Div)-StartMicros < us));
}

void LCD_Delay_ms(uint8_t ms)
{
  #if _LCD_USE_FREERTOS==1
  osDelay(ms);
  #else
  HAL_Delay(ms);
  #endif
}
