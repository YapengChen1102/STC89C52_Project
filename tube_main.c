#include<REGX52.h>
#include"Nixie_Display.h"

unsigned int Hour=0, Min=0, Sec=0; // 程序设置初始时间

/**
	* @brief 	定时器T0初始化
	*/
void Timer0_Init(void)
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式,定时器模式1,16位计数器
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}


void main(){
	Timer0_Init();
		while(1)
		{
			ShowNum(7, Sec, 2);
			ShowChar(6, '-');
			ShowNum(4, Min, 2);
			ShowChar(3, '-');
			ShowNum(1, Hour, 2);
		}
}

/**
	* @brief 	定时器T0中断函数
	*/
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18; 	// T0低八位的初值
	TH0 = 0xFC;		// T0高八位的初值
	T0Count++;
	if(T0Count >= 1000)
	{
		T0Count = 0;
		Sec++;
		
		if(Sec >= 60){ Sec = 0; Min++; }
		
		if(Min >= 60){ Min = 0; Hour++; }
		
		if(Hour >= 24){ Hour = 0; }
	}
}