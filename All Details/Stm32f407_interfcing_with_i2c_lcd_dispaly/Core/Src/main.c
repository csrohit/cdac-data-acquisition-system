#include "stm32f4xx_hal.h"
#include "STM32F407_I2C_LCD16x02_Driver.h"

int main(void)
{
	//HAL Initialise
	HAL_Init();

   //Initialize LCD
	LCD_Init();

	LCD_Send_String("Hi I Am From ");
	LCD_Send_Cmd(LCD_SET_ROW2_COL1);
	LCD_Send_String("     PUNE    ");
  HAL_Delay(1000);
	LCD_Display_Long_Message("My Name Is Prateek Singh :-)");
	while(1)
	{


	}

}
