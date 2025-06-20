#include <REGX52.H>
#include <INTRINS.H>

void Delay500ms()
{
	unsigned char i,j,k;
	_nop_();
	i = 4;
	j = 205;
	k = 187;
	do
	{
		do
		{
			while(--k);
		}while(--j);
	}while(--i);
}


void main()
{
	while(1)
	{
		P2 &= 0x1F;
		P2 |= 0x80;
		P0 = 0xFE;
		Delay500ms();
		P0 = 0xFF;
		Delay500ms();
	}
}
