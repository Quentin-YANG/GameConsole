/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   Quentin Yang
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define byte unsigned char 

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF

/*LCD MACROs*/
#define CMD_PAGE 0xB0
#define CMD_COL_LSB 0x00
#define CMD_COL_MSB 0x10
//#define ROW 32
//#define COLUMN 51
//#define PIXEL 0x80
#define MAX_ROW 63
#define MAX_COLUMN 102     //Max Used Columns 0 to 101
#define MAX_PAGE 8         //Pages 0 to 7
#define TOTAL_COLUMNS 132  // Total Columns 0 to 131
#define LCD_CLEAR 0x00
#define LCD_BLACK 0xA7
#define LCD_WHITE 0xA6

#define INT_FLAG GET(GIFR,_BV(INTF1))

//----------------SET and GET MACRO---------------------------
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK

//----------------Devices Direction MACROs-------------------
//#define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
//LED Direction MACROs
#define BAT_LOW_LED_DIR(DIR) SET(DDRA,_BV(PA0),DIR)

//BUTTON Direction MACROs
#define UP_BUTTON_DIR(DIR) SET(DDRA,_BV(PA1),DIR)
#define DOWN_BUTTON_DIR(DIR) SET(DDRA,_BV(PA2),DIR)
#define LEFT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA3),DIR)
#define RIGHT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA4),DIR)
#define KEY1_BUTTON_DIR(DIR) SET(DDRD,_BV(PD0),DIR)
#define KEY2_BUTTON_DIR(DIR) SET(DDRD,_BV(PD1),DIR)
#define KEY3_BUTTON_DIR(DIR) SET(DDRD,_BV(PD2),DIR)

//LCD Direction MACROs
#define LCD_DATA_COMMAND_DIR(DIR) SET(DDRB,_BV(PB2),DIR)
#define LCD_CS0_DIR(DIR) SET(DDRB,_BV(PB0),DIR)
#define LCD_BACKLIGHT_DIR(DIR) SET(DDRB,_BV(PB3),DIR)
#define LCD_RESET_DIR(DIR) SET(DDRB,_BV(PB1),DIR)

//RAM Direction MACROs
#define RAM_CS_DIR(DIR) SET(DDRC,_BV(PC0),DIR)
#define RAM_WP_DIR(DIR) SET(DDRC,_BV(PC1),DIR)
#define RAM_HOLD_DIR(DIR) SET(DDRC,_BV(PC6),DIR)

//COMMON PINS Direction
#define SS_DIR(DIR) SET(DDRB,_BV(PB4),DIR)
#define MOSI_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
#define MISO_DIR(DIR) SET(DDRB,_BV(PB6),DIR)
#define SCK_DIR(DIR) SET(DDRB,_BV(PB7),DIR)

//BUTTON INIT MACROS
#define BUTINT_DIR(DIR) SET(DDRD,_BV(PD3),DIR)

//----------------Devices Outputs---------------------------
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)

//LED PINS OUTPUTS
#define BAT_LOW_LED(STATE) SET(PORTA,_BV(PA0),~STATE)

//BUTTON PINS OUTPUTS
#define UP_BUTTON_SET(STATE) SET(PORTA,_BV(PA1),STATE)
#define DOWN_BUTTON_SET(STATE) SET(PORTA,_BV(PA2),STATE)
#define LEFT_BUTTON_SET(STATE) SET(PORTA,_BV(PA3),STATE)
#define RIGHT_BUTTON_SET(STATE) SET(PORTA,_BV(PA4),STATE)
#define KEY1_BUTTON_SET(STATE) SET(PORTD,_BV(PD0),STATE)
#define KEY2_BUTTON_SET(STATE) SET(PORTD,_BV(PD1),STATE)
#define KEY3_BUTTON_SET(STATE) SET(PORTD,_BV(PD2),STATE)

//LCD PINS OUTPUTS
#define LCD_DATA_COMMAND_STATE(STATE) SET(PORTB,_BV(PB2),STATE)
#define LCD_CS0_STATE(STATE) SET(PORTB,_BV(PB0),STATE)
#define LCD_BACKLIGHT_STATE(STATE) SET(PORTB,_BV(PB3),STATE)
#define LCD_RESET_STATE(STATE) SET(PORTB,_BV(PB1),STATE)

//RAM PINS OUTPUTS
#define RAM_CS_STATE(STATE) SET(PORTC,_BV(PC0),STATE)
#define RAM_WP_STATE(STATE) SET(PORTC,_BV(PC1),STATE)
#define RAM_HOLD_STATE(STATE) SET(PORTC,_BV(PC6),STATE)

//COMMON PINS OUTPUTS
#define SS_STATE(STATE) SET(PORTB,_BV(PB4),STATE)
#define MOSI_STATE(STATE) SET(PORTB,_BV(PB5),STATE)
#define MISO_STATE(STATE) SET(PORTB,_BV(PB6),STATE)
#define SCK_STATE(STATE) SET(PORTB,_BV(PB7),STATE)

//BUTTON INIT PIN OUTPUT
#define BUTINT_STATE(STATE) SET(PORTD,_BV(PD3),STATE)

//--------------Devices Inputs---------------------------
//BATT INPUTS
#define BAT_LOW_LED_READ GET(PINA,_BV(PA5))

//BUTTON INPUTS
#define UP_BUTTON GET(PINA,_BV(PA1))
#define DOWN_BUTTON GET(PINA,_BV(PA2))
#define LEFT_BUTTON GET(PINA,_BV(PA3))
#define RIGHT_BUTTON GET(PINA,_BV(PA4))
#define KEY1_BUTTON GET(PIND,_BV(PD0))
#define KEY2_BUTTON GET(PIND,_BV(PD1))
#define KEY3_BUTTON GET(PIND,_BV(PD2))

//BUTTON INIT INPUTS
#define BUTINT GET(PIND,_BV(PD3))

//---------------Function Declearation------------------
void LCD_Command_TX(byte command);
void LCD_Data_TX(byte data);
void LCD_clear(void);
void Select_column (byte coumn);
void Select_page (byte page);
void SPI_MasterTrans(byte cData);
void PWM_init(void);



//------------------Game MACROs-------------------------
#define SNAKE_MAX_LENGTH 20
#define SNAKE_UP 1
#define SNAKE_DOWN 2
#define SNAKE_LEFT 3
#define SNAKE_RIGHT 4

#define INIT_LEVEL 5
#define INIT_BODY_LENGTH 6
#define SNAKE_SPEED_DELAY 700
#define BULLET_LENGTH 1

byte Hit_boarder(byte row,byte column,byte level);
byte Hit_body(byte row,byte column,byte body_index,byte *x_buffer,byte *y_buffer);
byte Bullet_hit(byte row,byte column,byte x);








