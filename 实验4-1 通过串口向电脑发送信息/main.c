#include <REGX52.H>
sfr AUXR = 0x8E;

void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//Baudrate no doubled
	SCON = 0x50;		//8 bits and variable baudrate
	AUXR |= 0x40;		//imer clock is 1T mode
	AUXR &= 0xFE;		//UART 1 use Timer1 as baudrate generator
	TMOD &= 0x0F;		//Set timer work mode
	TMOD |= 0x20;		//Set timer work mode
	TL1 = 0xDC;			//Initial timer value
	TH1 = 0xDC;			//Set reload value
	ET1 = 0;			//Disable Timer%d interrupt
	TR1 = 1;			//Timer1 start run
	ES = 1;
	EA = 1;
}


void UART_Send(unsigned char Byte)
{
	SBUF  = Byte;
	while(TI == 0);
	TI = 0;
}


void main()
{
	Uart1_Init();
	while(1)
	{
		
	}
}

void UART_Routine(void) interrupt 4
{
	if(RI == 1)
	{
		UART_Send(SBUF);
		RI = 0;
	}
}

