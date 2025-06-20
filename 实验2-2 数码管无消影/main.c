/**
	* 八位数码管显示--无消影程序
	* 实验现象： 数字重叠，同时显示两位或多位相同的数字，出现鬼影
	* 原因: 现实系统是有延时的 + 数码管动态扫描 不加消影是必然的。
	*/
	
#include <REGX52.H>

/**
	* 设置显示时间
	*/
unsigned int Hour=00, Min=00, Sec=00;



unsigned int refresh = 0; 
// digit display ( 0~9 )
unsigned char digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
												 0x92, 0x82, 0xF8, 0x80, 0x90};	

// 八位数码管选通
unsigned char tube_label[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
												 
void delay(unsigned int time)
{
	unsigned i,j;
	for(i=0; i<time; i++)
	{
		for(j=0; j<=120; j++);
	}
}

void Bit_Latch()
{
	P2 = (P2 & 0x1F)|0xC0;
	P2 &= 0x1F;
}


void Seg_Latch()
{
	P2 = (P2 & 0x1F)|0xE0;		
	P2 &= 0x1F;								
}

int Power(int X, int Y)
{
	int i, result=1;
	for(i=0; i<Y; i++)
	{
		result *= X;
	}
	return result;
}


void ShowNum(unsigned int column, unsigned int number, unsigned int length)
{
	unsigned int i, loc = column-1;
	for(i=length; i>0; i--)
	{
		P0 = tube_label[loc];
		Bit_Latch();
		P0 = digit[(number/Power(10, i-1))%10];
		Seg_Latch();
		loc++;
		delay(refresh);
	}
}

void ShowChar(unsigned int column, unsigned char Char)
{
	if(Char=='-')
	{
		P0 = tube_label[column-1];
		Bit_Latch();
		P0 = 0xBF;
		Seg_Latch();
		delay(refresh);
	}
}

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
