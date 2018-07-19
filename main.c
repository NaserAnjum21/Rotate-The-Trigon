/*
 * MCU_Project.c
 *
 * Created: 7/15/2018 8:39:31 PM
 * Author : ASUS
 */ 

#define F_CPU 1000000

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#define loop 10000

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h" //Can be download from the bottom of this article

int colors[3][8]={{0,1,2,3,4,5,6,7},{16,17,18,19,20,21,22,23},{24,25,26,27,28,29,30,31}};

int col[8][8];

int color_select[8];

int gap=0;

int top=0;
int left=2;
int right=3;

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

uint8_t clm[8]={1,2,4,1,4,2,1,2};

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
			for(int j=0;j<1000;j++)
			{
				PORTA=a*8+3;
				PORTA=a*8+4;
			}
		}
		if(i==7)
		{
			for(int j=0;j<1000;j++)
			{
				PORTA=b*8+2;
				PORTA=b*8+3;
				PORTA=c*8+4;
				PORTA=c*8+5;
			}
		}
	}
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
	gap++;
	for(int i=7;i>0;i--)
	{
		clm[i]=clm[i-1];
	}
	int cnt=rand()%3;
	int prod=1;
	for(int k=0;k<cnt;k++) prod=prod*2;
	if(gap>=3)
	{
		clm[0]=prod; gap=0;
	}
	else
		clm[0]=0; 

	for(int j=0;j<30;j++)
	{
		for(int i=0;i<8;i+=1)
		{
			PORTD=row[i];
			PORTA=~clm[i];
			_delay_ms(2.5);
		}
		
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
		_delay_ms(200);
	}
	if(PINB & 0b00000010)
	{
		right_rotate();
		//trigon(top,left,right);
		_delay_ms(200);
	}
	trigon(top,left,right);
}

void sliding()
{

	for(int i=0;i<8;i++)
	{
		color_select[i]=rand()%3;
	}

	while(1)
	{

		for(int i=7;i>0;i--)
		{
			color_select[i]=color_select[i-1];
		}
		color_select[0]=rand()%3;

		
		int t=rand()%3;
		
		for(int j=0;j<50;j++)
		{
			for(int i=0;i<8;i++)
			{
				//if(i%2) PORTD=row[i];
				//else PORTD=0;
				//color_select[i]
				PORTD=row[i];
				full_line_color(color_select[i]);
				
				_delay_ms(2);
			}
		}

		
		
		//_delay_ms(1000);
		
	}

}

int main(void)
{
	DDRA=0xFF;
	DDRD=0xFF;
	DDRB=0x00;
	uint16_t count=0;

	

	//PORTD=0b0000001;
	while(1)
	{
		//scrolling();
		//sliding();

		func_trigon();
		
	}
	

	

}

/*



*/


