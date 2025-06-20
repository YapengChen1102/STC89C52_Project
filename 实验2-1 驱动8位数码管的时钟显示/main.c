#include <REGX52.H>
#include "Nixie_Disp.h"

unsigned int Hour=00, Min=00, Sec=00;


void Timer0_Init()
{

	TMOD &= 0xF0;			//Set timer work mode
	TMOD |= 0x01;			//Set timer work mode
	TL0 = 0x18;				//Initial timer value
	TH0 = 0xFC;				//Initial timer value
	TF0 = 0;				//Clear TF0 flag
	TR0 = 1;				//Timer0 start run
	ET0 = 1;
	EA  = 1;
}


void main()
{	
	P0 = 0xFF;
	P2 = (P2 & 0x1F) | 0x80;
	P2 &= 0x1F;
	
	Timer0_Init();
	while(1)
	{
		ShowNum(1, Hour, 2);
		ShowChar(3, '-');
		ShowNum(4, Min, 2);
		ShowChar(6, '-');
		ShowNum(7, Sec, 2);
	}
}


void Timer0_Routine() interrupt 1
{
	static unsigned int count;
	TL0 = 0x18;
	TH0 = 0xFC;
	if(count >= 1000)
	{
		count = 0;
		Sec++;
		if(Sec==60){ Min++; Sec = 0; }
		if(Min==60){ Hour++; Min = 0; }
		if(Hour==24){ Hour = 0; }
	}
	
	count++;
}
