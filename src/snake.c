/* This file includes code related to the game of snake */

#include "game_console.h"

typedef struct
{
	byte x;
	byte y;
	char vx;
	char vy;

}bullet_t;

typedef struct
{
	byte x;
	byte y;
}dot_t;

byte frame_buffer[MAX_COLUMN][MAX_PAGE]={0};  //Game Field
byte page;
byte column;
byte row;
byte pixel;
byte body_index;
byte body_length;
byte Direction = SNAKE_UP;


void Snake_Game(void)
{
	byte x_buffer[SNAKE_MAX_LENGTH];
	byte y_buffer[SNAKE_MAX_LENGTH];
	
	LCD_clear();
	Buffer_Init();
	xy_Buffer_Init(y_buffer,x_buffer);
	byte bullet_x_buffer[3];
	byte bullet_y_buffer[3];
	byte bullet_idle = TRUE;
	byte bullet_index;
	byte level = INIT_LEVEL;
	byte food_eat = FALSE;
	byte body_hit = FALSE;
	int speed_cnt = 0;
	int speed_delay = SNAKE_SPEED_DELAY;
	
	dot_t food;
	bullet_t bullet;
	bullet.x = 0;
	bullet.y = 0;
	bullet.vx = 0;
	bullet.vy = 0;
	
	body_index = 0;
	body_length = INIT_BODY_LENGTH;
	rand_food(&food,level);
	Draw(food.x,food.y);
	
	row = 33;			//Init pixel to central of screen
	column = 50;

	byte i;
	for(i=1;i<=level;i++)Draw_boarder(i);

	while(TRUE){
//-----------------Speed Control-----------------------
		if(speed_cnt >= speed_delay){
		speed_cnt=0;
		if(body_hit == FALSE)
		{
			switch(Direction)
			{
				case SNAKE_UP:
					row--;
				    break;
				case SNAKE_DOWN:
					row++;
				    break;
				case SNAKE_LEFT:
					column--;
				    break;
				case SNAKE_RIGHT:
					column++;
				    break;
				default:
					break;
			}
		}
		body_hit = FALSE;
				
		if(Hit_boarder(row,column,level)||Hit_body(row,column,body_index,x_buffer,y_buffer))
		{
			if((frame_buffer[column][row/8])&(_BV(row%8))||Hit_boarder(row,column,1))
			{
				LCD_clear();
				Buffer_Init();
				xy_Buffer_Init(y_buffer,x_buffer);
				level = INIT_LEVEL;
				Direction = SNAKE_UP;
				body_index = 0;
				body_length = INIT_BODY_LENGTH;
				rand_food(&food,level);
				Draw(food.x,food.y);
	
				row = 33;			//Init pixel to central of screen
				column = 50;

				food_eat = FALSE;

				int clean_cnt;
				for(clean_cnt=0;clean_cnt<SNAKE_MAX_LENGTH;clean_cnt++)
				{
					Clear_pixel(y_buffer[clean_cnt],x_buffer[clean_cnt]);
					y_buffer[clean_cnt]=0;
					x_buffer[clean_cnt]=0;	
				}
				for(i=1;i<=level;i++)Draw_boarder(i);
			}
		}


	Draw(row,column);
	x_buffer[body_index] = column;
	y_buffer[body_index] = row;

	if(body_index>=body_length)
	{
		int i,j;
		for(j=0;j<body_index-body_length+1;j++)
		{
			Clear_pixel(y_buffer[0],x_buffer[0]);
			for(i=0;i<body_index;i++)
			{
				x_buffer[i]=x_buffer[i+1];
				y_buffer[i]=y_buffer[i+1];
			}
		}
		body_index=body_length;
	}
	else
	{
		body_index++;
	}
  }

//UP,DOWN,LEFT,RIGHT and KEY1 set in interrupt function
	

	if((row == food.x)&&(column == food.y))
	{
		if(body_length < SNAKE_MAX_LENGTH - 1){
				body_length++;
				food_eat = TRUE;
			}
	}

	if(food_eat == TRUE)
	{
		level++;
		rand_food(&food,level);
		Draw_boarder(level);
		food_eat = FALSE;
		Draw(food.x,food.y);
	}

	Draw(food.x,food.y);

	if(~KEY2_BUTTON)            //Fire a bullet
	{
		if(body_length > 3)
		{
			if(bullet_idle == TRUE)
			{
				bullet_idle = FALSE;
				body_length--;

				byte speed = 1;
				switch(Direction)
				{
					case SNAKE_UP:
						bullet.vx = 0;
						bullet.vy = -speed;
						bullet.x = column;
						bullet.y = row-1;
					    break;
					case SNAKE_DOWN:
						bullet.x = 0;
						bullet.vy = speed;
						bullet.x = column;
						bullet.y = row+1;
						break;
					case SNAKE_LEFT:
						bullet.vx = -speed;
						bullet.vy = 0;
						bullet.x = column-1;
						bullet.y = row;
						break;
					case SNAKE_RIGHT:
						bullet.vx = speed;
						bullet.vy = 0;
						bullet.x = column+1;
						bullet.y = row;
						break;
					default:
						break;
				}
			}
		}
	}

	if(~KEY3_BUTTON){
		speed_delay = 300;
	}
	else{
		speed_delay = SNAKE_SPEED_DELAY;
	}

	if(bullet_idle == FALSE){

		if((bullet.y==food.x)&&(bullet.x==food.y))  //if bullet hits food
		{
			food_eat = TRUE;
			body_length++;
			column = bullet.x;
			row = bullet.y;
			bullet.x=0;
			bullet.y=0;
			bullet_idle = TRUE;
			bullet.vx=0;
			bullet.vy=0;
			bullet_index=0;
			body_hit = TRUE;
		}

		bullet.x += bullet.vx;
		bullet.y += bullet.vy;
		
		if(Bullet_hit(bullet.y,bullet.x,level))
		{
			if((frame_buffer[bullet.x][bullet.y/8])&(_BV(bullet.y%8)))
			{
				byte j;
				for(i=0;i<3;i++)Clear_pixel(bullet.y+2,bullet.x+1-i);
				for(j=0;j<3;j++)
				{
					for(i=0;i<5;i++)Clear_pixel(bullet.y+1-j,bullet.x+2-i);
				}
				for(i=0;i<3;i++)Clear_pixel(bullet.y-2,bullet.x+1-i);
				bullet_idle = TRUE;
				bullet.x=0;
				bullet.y=0;
				bullet.vx =0;
				bullet.vy =0;
				bullet_index=0;
			}
		}

		Draw(bullet.y,bullet.x);

		bullet_x_buffer[bullet_index] = bullet.x;
		bullet_y_buffer[bullet_index] = bullet.y;

		if(bullet_index>=BULLET_LENGTH)
		{
			Clear_pixel(bullet_y_buffer[BULLET_LENGTH-1],bullet_x_buffer[BULLET_LENGTH-1]);
				
			for(i=0;i<BULLET_LENGTH;i++)
			{
				bullet_x_buffer[i] = bullet_x_buffer[i+1];
				bullet_y_buffer[i] = bullet_y_buffer[i+1];
			}
		}
		else
		{
			bullet_index++;
		}
	}

	speed_cnt++;
	

  }

}

void Buffer_Init(void)
{
	byte i;
	byte j;
	for (i=0;i<MAX_PAGE;i++)
	{
		for(j=0;j<MAX_COLUMN;j++)
		{
			frame_buffer[j][i]=LCD_CLEAR;
		}
	}	
}

void xy_Buffer_Init(byte *y_buffer,byte *x_buffer)
{
	byte i;
	for(i=0;i<SNAKE_MAX_LENGTH;i++)
	{
		y_buffer[i]=0;
		x_buffer[i]=0;
	}
}

void rand_food(dot_t *food, byte level)
{
	food->y = rand()%(MAX_COLUMN - 2*level - 3) + level + 1;
	food->x = rand()%(MAX_ROW - 2*level - 2) + level + 1;
}

void Draw(byte row,byte column)
{

	page = row/8;  //Find page the Head belongs
	pixel = row%8; //Find position of pixel
	Select_page(page);
	Select_column(column);
	pixel = (_BV(pixel)|(frame_buffer[column][page]));
	frame_buffer[column][page] = pixel;
	LCD_Data_TX(pixel);
}

void Draw_boarder(byte j)
{
	j--;
	int i;
	for(i=0+j;i<64-j;i++){
		Draw(i,101-j);
	}
	for(i=0+j;i<64-j;i++){
		Draw(i,0+j);
	}
	for(i=0+j;i<102-j;i++){
		Draw(0+j,i);
	}
	for(i=0+j;i<102-j;i++){
		Draw(63-j,i);
	}
}	


void Clear_pixel(byte row,byte column)
{
	page = row/8;
	pixel = row%8;
	Select_page(page);
	Select_column(column);
	pixel = frame_buffer[column][page]&~_BV(pixel);	
	frame_buffer[column][page] = pixel;
	LCD_Data_TX(pixel);
}

byte Hit_boarder(byte row,byte column,byte level)
{
	if((row<level)||(row>MAX_ROW-level)||(column<level)||(column>MAX_COLUMN-level))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

byte Hit_body(byte row,byte column,byte body_index,byte *x_buffer,byte *y_buffer)
{
	byte i;
	if(x_buffer[4]!=0)
	{
		for(i=0;i<body_index-1;i++)
		{
			if(y_buffer[i]==row && x_buffer[i]==column)
			return TRUE;
		}
		return FALSE;
	}
}

byte Bullet_hit(byte row,byte column,byte x)
{
	if(row<x||row>MAX_ROW-x||column<x||column>MAX_COLUMN-x-1){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void FRAM_Init(void)
{
	RAM_CS_DIR(OUT);
	RAM_CS_STATE(LOW);
}

ISR(INT1_vect)
{
//--------------Button Scan---------------------------

	if(~UP_BUTTON)
	{
		if(Direction != SNAKE_DOWN) Direction = SNAKE_UP;
	}
	if(~DOWN_BUTTON)
	{
		if(Direction != SNAKE_UP) Direction = SNAKE_DOWN;
	}
	if(~LEFT_BUTTON)
	{
		if(Direction != SNAKE_RIGHT) Direction = SNAKE_LEFT;
	}
	if(~RIGHT_BUTTON)
	{
		if(Direction != SNAKE_LEFT) Direction = SNAKE_RIGHT;
	}

//---------------LCD Brightness Adjust-------------------
	if(~KEY1_BUTTON)
	{
		OCR0 = (OCR0==0)?1:(OCR0*4);
	}
}
