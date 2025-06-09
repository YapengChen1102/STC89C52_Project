#ifndef __Nixie_Display_H__
#define __Nixie_Display_H__

unsigned int refresh = 1; // *ms 后刷新
unsigned char tube_label[] = {0x00, 0x04, 0x08, 0x0c, 0x10, 0x14, 0x1b, 0x1c};	// 138译码器
unsigned char arr[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; // 数字0~9共阴极段码


void delay(unsigned int n)
{
	unsigned int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<120; j++);
	}
}


/**
	* @recall 	Power(X, Y)
	* @brief 		幂函数，用于取数问题
	* @param 1 	X是底数
	* @param 2 	Y是幂数
	* @demo 		Power(2, 3) = 8
	*/
int Power(int X, int Y)
{
	int i, result = 1;
	for(i=0; i<Y; i++)
	{
		result *= X;
	}
	return result;
}


/**
	* @brief 		在数码管上显示数字
	* @param 1 	在第几列显示 1~8
	* @param 2	要显示的数字 0~65535
	* @param 3 	显示数字的长度 1~8
	*/
void ShowNum(unsigned int column, unsigned int number, unsigned int length)
{
	int i, location = column-1;
	for(i=length; i>0; i--)
	{
		P0 = arr[number/Power(10, i-1)%10];
		P2 = tube_label[location];
		location++;
		delay(refresh);
	}
}

/**
	* @brief 		在数码管上显示字符
	* @param 1 	在第几列显示
	* @param 2  要显示的字符
	* @remark 	可显示的字符有限，如'-'
	*/

void ShowChar(unsigned int column, unsigned char Char)
{
	if(Char == '-')
	{
		P2 = tube_label[column-1];
		P0 = 0x40;
		delay(refresh);
	}
}

#endif