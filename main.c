/*
 * MCU_Project.c
 *
 * Created: 7/15/2018 8:39:31 PM
 * Author : ASUS
 */ 

#define F_CPU 1000000

/*#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7*/

#define D4 eS_PORTC4
#define D5 eS_PORTC5
#define D6 eS_PORTC6
#define D7 eS_PORTC7
#define RS eS_PORTC0
#define EN eS_PORTC1

#define loop 1

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "lcd.h"
//#include "lcd2.h"

int colors[3][8]={{0,1,2,3,4,5,6,7},{16,17,18,19,20,21,22,23},{24,25,26,27,28,29,30,31}};

int col[8][8];

int color_select[8];

int gap=0;
int gap2=0;

int gap_offset=6;
int level_offset=5;

int got_it=0;
int game_over=0;
int line_gate=1;

int score=0;
char score_str[6];

int top=0;
int left=2;
int right=3;

int cur_state=1;
int prev_state=1;

int level=0;

uint16_t red[]={0,1,2,3,4,5,6,7};
//uint16_t red[]={8,9,10,11,12,13,14,15};
uint16_t green[]={16,17,18,19,20,21,22,23};
uint16_t blue[]={24,25,26,27,28,29,30,31};



uint8_t row[8] = {0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
0b10000000};

uint8_t clm[8]={0,0,0,0,0,0,0,0};

void full_line(int ro)
{
	int temp=rand()%3;
	for(int j=0;j<loop;j++)
	{
		for(int i=0;i<8;i++)
		{
			PORTA=colors[temp][i];
			col[ro][i]=colors[temp][i];
			//_delay_ms(2);
		}
		
	}
	PORTA=8;
	//_delay_ms(300);
}

void full_line_color(int n)
{
	if(n==-1)
	{
		for(int i=0;i<8;i++)
		{
			PORTA=8;
		}
		return;
	}
	for(int j=0;j<loop;j++)
	{
		for(int i=0;i<8;i++)
		{
			PORTA=colors[n][i];
			//col[ro][i]=colors[temp][i];
			//_delay_ms(2);
		}
		
	}
	PORTA=8;

}

void trigon(int a,int b,int c)
{
	for(int i=6;i<8;i++)
	{
		PORTD=row[i];
		if(i==6)
		{
			for(int j=0;j<1;j++)
			{
				PORTA=a*8+3;
				PORTA=a*8+4;
			}
		}
		PORTA=8;
		if(i==7)
		{
			for(int j=0;j<1;j++)
			{
				PORTA=b*8+2;
				PORTA=b*8+3;
				PORTA=c*8+4;
				PORTA=c*8+5;
			}
		}
	}
	PORTA=8;
}

void left_rotate()
{
	int temp=right;
	right=left;
	left=top;
	top=temp;
}

void right_rotate()
{
	int temp=left;
	left=right;
	right=top;
	top=temp;
}

void scrolling()
{
	if(PINC & 0x01)
	{
		return;
	}

	if(PINC & 0x02)
	{
		if(gap_offset-1>=3)
		{
			gap_offset--;
		}
	}

	gap++;
	for(int i=7;i>0;i--)
	{
		clm[i]=clm[i-1];
	}
	int cnt=rand()%3;
	int prod=1;
	for(int k=0;k<cnt;k++) prod=prod*2;
	if(gap>=gap_offset)
	{
		clm[0]=prod; gap=0;
	}
	else
		clm[0]=0; 

	int temp=clm[7];

	if(temp==4) PORTB=temp-1;
	else PORTB=temp;

	for(int j=0;j<50;j++)
	{
		for(int i=0;i<8;i+=1)
		{
			PORTD=row[i];
			PORTA=~clm[i];

			_delay_ms(1.5);
		}
		if(temp==1) PORTB=1;
		else if(temp==2) PORTB=2;
		else if(temp==4) PORTB=3;
		else PORTB=0;
		PORTD=0;
	}
	
	//PORTD=0;
}

void func_trigon()
{
	if(PINB & 0b00000001)
	{
		left_rotate();
		//trigon(top,left,right);
		//_delay_ms(200);
	}

	if(PINB & 0b00000010)
	{
		right_rotate();
		//trigon(top,left,right);
		//_delay_ms(200);
	}

	/*if(PINC==0) //tilt
	{
		left_rotate();
		//trigon(top,left,right);
		//_delay_ms(200);
	}*/

	trigon(top,left,right);
}

void sliding()
{

	if(game_over)
	{
		PORTB |= (1<<5);
		//cross();
		
	}

	else
	{
		PORTB &= (~(1<<5));
		
		int get_color= (PINB & 0b00001100) >> 2;

		int valid=1;

		if(get_color>=1 && get_color<=3) got_it=1;
		//else got_it=0;

		//func_trigon();

		for(int k=0;k<1000;k++)
		{
			//func_trigon();
			trigon(top,left,right);

			//get_color= (PINB & 0b00001100) >> 2;
			//get_color= rand()%3+1;
			//if(get_color==2) get_color=1;
			//else if(get_color==1) get_color=2;

			if(k==0)
			{
				get_color= (PINB & 0b00001100) >> 2;
				//get_color= rand()%3+1;

				if(get_color>=1 && get_color<=3) valid=1;
				else valid=0;

				if(get_color==2) get_color=1;
				else if(get_color==1) get_color=2;


				gap2++;
				
				for(int i=7;i>0;i--)
				{
					color_select[i]=color_select[i-1];
				}
				color_select[0]=get_color-1;
				
			}
			if(color_select[0]>=0 && color_select[1]>=0) color_select[0]=-1; //double line removal

			//trigon(top,left,right);

			for(int i=0;i<6;i++)
			{
				//if(i%2) PORTD=row[i];
				//else PORTD=0;
				//color_select[i]
				
				if(got_it) PORTD=row[i];
				else PORTD=0;
				full_line_color(color_select[i]);
				if(gap2 >= 3)
				{
					gap2=0; 
				}
				//else full_line_color(-1);
				
				//_delay_ms(2);
			}
			
		}	

		PORTB &= (~(1<<6));
		
		int last_color=color_select[5];

		if(last_color>=1) last_color++;
		
		if(top!=last_color && last_color>=0)
		{
			game_over=1;
			Lcd4_Clear(); Lcd4_Set_Cursor(1,0); Lcd4_Write_String("GAME OVER"); 
			//LCD_Clear(); LCD_String("GAME OVER"); // LCD_String("Score: ");	LCD_Command(0xC0);	LCD_String(score_str);
			
		}
		else if(last_color>=0 && !game_over)
		{
			score++;
		}
		if(level < score/level_offset)
		{
			level=score/level_offset;
			PORTB |= (1<<6);
		}


		//_delay_ms(1000);
		
	}
	
}

void tilt_sensor_check()
{
	if(PINB & 0b00010000) //tilt
	{
		//PORTC=1;
		//_delay_ms(200);
	}
	else
	{
		left_rotate();
	} //PORTC=0;

	if(PINB & 0b00000001)
	{
		left_rotate();
		//trigon(top,left,right);
		//_delay_ms(200);
	}

	if(PINB & 0b00000010)
	{
		//right_rotate();
		//trigon(top,left,right);
		//_delay_ms(200);
	}
	if(!game_over) trigon(top,left,right);
}

void init()
{
	PORTA=8;
	PORTD=0;
	PORTC=4;
	PORTB=0;

}

void lcd2()
{
	LCD_Init();			/* Initialization of LCD*/

	LCD_String("Score: ");	LCD_Command(0xC0);	LCD_String(score_str);

}

void lcd1()
{
	//Lcd4_Init();
	
	
}

void cross(){
	for(int j=0;j<1000;j++)
	{
		for( int i = 0; i < 8; i++ ){
			PORTD = row[i];
			PORTA = i;
			//_delay_ms(2);
			PORTA = 7-i;
			//_delay_ms(2);
		}
		PORTA=8;
	}
	
	//_delay_ms(1000);
}

void lower_mcu()
{
	DDRB=0b01100000;
	DDRC=0xFF;
}

void upper_mcu()
{
	DDRB=0xFF;
	DDRC=0x00;
}

char R_array[3],W_array[3] = "10";

void high_score()
{
	memset(R_array,0,3);
	eeprom_read_block(R_array,0,strlen(W_array));
	
	int prev=atoi(R_array);
	if(prev < score)
	{
		itoa(score,R_array,10);
		eeprom_write_block(R_array,0,strlen(W_array));
	}
	Lcd4_Set_Cursor(2,4); Lcd4_Write_String("HS: "); Lcd4_Write_String(R_array);
}


int main(void)
{
	DDRA=0xFF;
	DDRD=0xFF;
	
	lower_mcu();
	//upper_mcu();

	//eeprom_write_block("5",0,strlen(W_array));
	

	uint16_t count=0;

	
	for(int i=0;i<8;i++)
	{
		color_select[i]=-1;
	}

	MCUCSR = (1<<JTD); MCUCSR = (1<<JTD);
	
	PORTB=0;
	

	Lcd4_Init(); Lcd4_Set_Cursor(1,1); Lcd4_Write_String("Score: ");
	//lcd2();
	//LCD_Init(); LCD_String("Score: ");	
	
	//PORTD=0b0000001;
	while(1)
	{
		//scrolling();

		itoa(score,score_str,10);
		Lcd4_Set_Cursor(2,1); Lcd4_Write_String(score_str);
		//LCD_Command(0xC0);	LCD_String(score_str);
		
		tilt_sensor_check(); sliding();
		
		if(game_over) high_score();
		//Lcd4_Clear();
	}
	

	

}

/*



*/


