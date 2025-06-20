#include <REGX52.H>

unsigned int loc = 0;
unsigned char arr[] = {0xFE, 0xFD, 0xFB, 0xF7,
					   0XEF, 0xDF, 0xBF, 0x7F,
					   0xBF, 0XDF, 0xEF, 0xF7,
					   0xFB, 0xFD};
											 
void Timer0_Init(void)		//50ms@12.000MHz
{
	TMOD &= 0xF0;			//Set timer work mode
	TMOD |= 0x01;			//Set timer work mode
	TL0 = 0xB0;				//Initial timer value
	TH0 = 0x3C;				//Initial timer value
	TF0 = 0;				//Clear TF0 flag
	TR0 = 1;				//Timer0 start run
	ET0 = 1;				//Enable Time0 Interrupt
	EA  = 1;				//Enable All
	PT0 = 0;				//Priority Low
}

void main()
{
	Timer0_Init();
	while(1)
	{
		P2 = (P2&0x1F)|0x80;
		P0 = arr[loc];
	}
}

void Timer0() interrupt 1
{
	static unsigned int counter;
	TL0 = 0xB0;				
	TH0 = 0x3C;
	counter++;
	
	while(counter >= 10)
	{
		counter = 0;
		loc++;
		if(loc >= 14) loc = 0;
	}
	
}
