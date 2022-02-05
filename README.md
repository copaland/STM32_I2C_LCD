# STM32_I2C_LCD

This I2C LCD library project file is implemented in STM32CubeIDE.

이 프로젝트의 I2C LCD 라이브러리 파일은 STM32CubeIDE 내에서 구현됩니다.

STM32 HAL library for LCD display based on 16x2 CLCD with PCF8574
Currently, the "stm32f4xx_hal.h" library is used.
You must include and use the appropriate library for your board.

The source and header files are placed in Src/liquidcrystal_i2c.c and Inc/liquidcrystal_i2c.h respectively.
After including the header file suitable for the STM32 MPU, write the Device I2C Address.

```
#include <LiquidCrystal_I2C.h>

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  
  LCD_Init();
  LCD_SetCursor(0,0);

  LCD_Print("Hello KiMSON");
  LCD_PrintSpecialChar(0); 
  
  LCD_Puts(0,1,"Linkbay.co.kr");
  LCD_PrintSpecialChar(1);
  
  while (1)
  {
	  // Shift Right 
	  for(int i=0; i<7; i++)
	  {
	    LCD_ScrollRight();
	    HAL_Delay(1000);
	  }
	  // Shift Left 
	  for(int i=0; i<7; i++)
	  {
	    LCD_ScrollLeft();
	    HAL_Delay(1000);
	  }
	  __NOP();
  }
}

```
