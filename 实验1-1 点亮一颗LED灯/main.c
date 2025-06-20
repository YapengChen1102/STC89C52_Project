#include <REGX52.H>

void main()
{
	while(1)
	{
		P2 &= 0x1F;
		P2 |= 0x80;
		P0 = 0xFE;
	}
}