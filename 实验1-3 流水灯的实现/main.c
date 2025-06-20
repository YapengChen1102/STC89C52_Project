#include <REGX52.H>
#include <INTRINS.H>


void Delay500ms()
{
	unsigned char i,j,k;
	_nop_();
	i = 23;
	j = 205;
	k = 120;
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
	int i = 0;
	while(1)
	{
		P2 &= 0x1F;
		P2 |= 0x80;
		P0 = 0xFE;
		for(i=1; i<=8; i++)
		{
			Delay500ms();
			P0 = _crol_(P0, 1);
		}
	}
}