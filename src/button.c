/* This file involves button init & button scan function */

#include "game_console.h"

void button_init(void)
{
	 UP_BUTTON_DIR(IN);
	 DOWN_BUTTON_DIR(IN);
	 LEFT_BUTTON_DIR(IN);
	 RIGHT_BUTTON_DIR(IN);
	 KEY1_BUTTON_DIR(IN);
	 KEY2_BUTTON_DIR(IN);
	 KEY3_BUTTON_DIR(IN);  //All functional buttons set as input

	 UP_BUTTON_SET(HIGH);
	 DOWN_BUTTON_SET(HIGH);
	 LEFT_BUTTON_SET(HIGH);
     RIGHT_BUTTON_SET(HIGH);
	 KEY1_BUTTON_SET(HIGH);
	 KEY2_BUTTON_SET(HIGH);
	 KEY3_BUTTON_SET(HIGH);  //Set all buttons init value

}

/* Interrupt init */
void Interrupt_init(void)
{
	 BUTINT_DIR(IN);       //Button interrupt pin set as input
	 BUTINT_STATE(LOW);   //Button interrupt pin init as low
	
	 MCUCR|=(1<<ISC11)|(1<<ISC10);  //posedge trigger
	 GICR|=(1<<INT1); //enable INT1
	 sei();
}

/* Button scan 
void Button_scan(void)
{
	extern byte row;
	extern byte column;
	extern byte confirm;

	if (~UP_BUTTON)
	{
		if(row>0)
		{
	    	row--;
			_delay_ms(100);
		}
	}

	if (~DOWN_BUTTON)
	{
		if(row<63)
		{
	    	row++;
			_delay_ms(100);
		}
	}

	if (~RIGHT_BUTTON)
	{
		if(column<MAX_COLUMN)
		{
	    	column++;
			_delay_ms(100);
		}
	}

	if (~LEFT_BUTTON)
	{
		if(column>0)
		{
	    	column--;
			_delay_ms(100);
		}
	}

	if(~KEY1_BUTTON)
	{
		OCR0 = (OCR0==0)?1:(OCR0*2);
		_delay_ms(255);
	}

	if(~KEY2_BUTTON)
	{
		confirm = ~confirm;
		_delay_ms(100);
	}

	if(~KEY3_BUTTON)
	{
		LCD_clear();
	}
}  */
