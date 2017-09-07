#include "game_console.h"

/* LCD init */
void LCD_init(void)
{
	LCD_DATA_COMMAND_DIR(OUT);  //LCD init
	LCD_CS0_DIR(OUT);
	LCD_RESET_DIR(OUT);

	LCD_RESET_STATE(LOW);
	_delay_ms(10);
	LCD_RESET_STATE(HIGH);
	
	LCD_Command_TX(0x40); //Display start line 0
	LCD_Command_TX(0xA1); //SEG reverse
	LCD_Command_TX(0xC0); //Normal COM0->COM63
	LCD_Command_TX(0xA4); //Set all pixel ON
	LCD_Command_TX(0xA6); //Display inverse off
	LCD_Command_TX(0xA2); //Set bias 1/9
	LCD_Command_TX(0x2F); //Power control(Booster,Regulator,Follower on)
	LCD_Command_TX(0x27); //Set VLCD Resistor Ratio
	LCD_Command_TX(0x81); 
	LCD_Command_TX(0x10); //Set Electronic Volume, default 32
	LCD_Command_TX(0xFA); 
	LCD_Command_TX(0x90); //Set Temperature compensation
	LCD_Command_TX(0xAF); //Display ON
}

void PWM_init(void)
{
    LCD_BACKLIGHT_DIR(OUT);  //LCD PWM init
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(0<<COM00)|(1<<COM01)|(1<<CS01); //Fast PWM Mode, non-inverting mode,clk 1/8 of crystal
	OCR0 = 64; //Medium level of light
}

void SPI_MasterInit(void)
{
	SS_DIR(OUT);
    MOSI_DIR(OUT); //Master output slaver input
    MISO_DIR(IN);  //Master input slaver output
    SCK_DIR(OUT);

	SS_STATE(HIGH); //idle
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0); //Enable SPI, Master, clock rate fck/16	
}

/* Master Transmit Function */
void SPI_MasterTrans(byte cData)
{
	SPDR = cData; //Enable trans
	while(!(SPSR & (1<<SPIF))); //Wait for trans complete
}

/* Send one byte data or command to LCD */
void LCD_Data_TX(byte data)
{
   LCD_CS0_STATE(LOW);//chip select
   LCD_DATA_COMMAND_STATE(HIGH);// SEND DATA
   SPI_MasterTrans(data);
   LCD_CS0_STATE(HIGH);//chip deselect
}

void LCD_Command_TX(byte command)
{
   LCD_CS0_STATE(LOW);//chip select
   LCD_DATA_COMMAND_STATE(LOW);// SEND COMMAND
   SPI_MasterTrans(command);
   LCD_CS0_STATE(HIGH);//chip deselect
}


/* Select Page */
void Select_page (byte page)
{
	byte page_cmd_address;
	//page = (page>MAX_PAGE)?(MAX_PAGE-1):page; //Forbid exceedance
	page_cmd_address =(CMD_PAGE | page);  
	LCD_Command_TX(page_cmd_address);        //Send the new page address
}

/* Select Column */
void Select_column (byte column)
{
	byte page_cmd_address_LSB;
	byte page_cmd_address_MSB;

	page_cmd_address_LSB =(CMD_COL_LSB | (column & 0x0F));  
	page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4));

	LCD_Command_TX(page_cmd_address_LSB);
	LCD_Command_TX(page_cmd_address_MSB);
}

/* Clear LCD, no pixel displays */
void LCD_clear(void)
{	
    byte clear_page=LCD_CLEAR;

	while(clear_page < MAX_PAGE)
	{
	    Select_page(clear_page);

		byte clear_column=LCD_CLEAR;
		while(clear_column < MAX_COLUMN)
		{
	    	Select_column(clear_column);
        	LCD_Data_TX(LCD_CLEAR);
			clear_column++;
		}
		clear_page++;
	}
}

