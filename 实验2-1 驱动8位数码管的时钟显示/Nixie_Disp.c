#include <REGX52.H>

unsigned int refresh = 5;
// digit display ( 0~9 )
unsigned char digit[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
												 0x92, 0x82, 0xF8, 0x80, 0x90};	

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