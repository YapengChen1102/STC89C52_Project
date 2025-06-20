#include <STC15F2K60S2.H>

char *string = "Hello Dr.";

void Uart1_Init(void)	//1200bps@12.000MHz
{
	PCON &= 0x7F;		//Baudrate no doubled
	SCON = 0x50;		//8 bits and variable baudrate
	AUXR &= 0xBF;		//Timer clock is 12T mode
	AUXR &= 0xFE;		//UART 1 use Timer1 as baudrate generator
	TMOD &= 0x0F;		//Set timer work mode
	TMOD |= 0x20;		//Set timer work mode
	TL1 = 0xE8;			//Initial timer value
	TH1 = 0xE8;			//Set reload value
	ET1 = 0;			//Disable Timer%d interrupt
	TR1 = 1;			//Timer1 start run
	EA 	= 1;
	ES 	= 1;
}


void UART_Send_Char(unsigned char Byte)
{
	SBUF  = Byte;
	while(TI == 0);
	TI = 0;
}

void UART_Send_String(char *str)
{
	while(*str)
	{
		UART_Send_Char(*str++);
	}
}

void UART_op(unsigned int SBUF)
{
	unsigned int temp = 0x01;
	switch(SBUF)
	{
		case 0x00:
			P0 = 0xFF;
			string = "All LEDs is OFF";
			break;
		/*-----------------------*/
		case 0x99:
			P0 = 0x00;
			string = "All LEDs is ON";
			break;
		/*-----------------------*/
		case 0x01:
			P00 = 0;
			string = "LED 1 is ON";
			break;
		case 0x10:
			P00 = 1;
			string = "LED 1 is OFF";
			break;
		/*-----------------------*/
		case 0x02:
			P01 = 0;
			string = "LED 2 is ON";
			break;
		case 0x20:
			P01 = 1;
			string = "LED 2 is OFF";
			break;
		/*-----------------------*/
		case 0x03:
			P02 = 0;
			string = "LED 3 is ON";
			break;
		case 0x30:
			P02 = 1;
			string = "LED 3 is OFF";
			break;
		/*-----------------------*/
		case 0x04:
			P03 = 0;
			string = "LED 4 is ON";
			break;
		case 0x40:
			P03 = 1;
			string = "LED 4 is OFF";
			break;
		/*------------------------*/
		case 0x05:
			P04 = 0;
			string = "LED 5 is ON";
			break;
		case 0x50:
			P04 = 1;
			string = "LED 5 is OFF";
			break;
		/*------------------------*/
		case 0x06:
			P05 = 0;
			string = "LED 6 is ON";
			break;
		case 0x60:
			P05 = 1;
			string = "LED 6 is OFF";
			break;
		/*------------------------*/
		case 0x07:
			P06 = 0;
			string = "LED 7 is ON";
			break;
		case 0x70:
			P06 = 1;
			string = "LED 7 is OFF";
			break;
		/*------------------------*/
		case 0x08:
			P07 = 0;
			string = "LED 8 is ON";
			break;
		case 0x80:
			P07 = 1;
			string = "LED 8 is OFF";
			break;
		default :
			string = "Invalid Command";
	}
}

void UART_Routine(void) interrupt 4
{
	if(RI == 1)
	{
		UART_op(SBUF);
		P2 = (P2&0x1F) | 0x80;
		P2 &= 0x1F;
		UART_Send_String(string);
		RI = 0;
	}
}

void main()
{
	Uart1_Init();
	while(1)
	{
		
	}
}
