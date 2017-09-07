/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE)
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/

#include "game_console.h"

int main(void)
{
	PWM_init();
	SPI_MasterInit();
	LCD_init();
	_delay_ms(255);
    LCD_clear();   

	ADC_init();
	button_init();
	Interrupt_init();
	FRAM_Init();

	while(TRUE)//Master loop always true so always loop
	{
		low_battery_detect();
		Snake_Game();
	}
}





