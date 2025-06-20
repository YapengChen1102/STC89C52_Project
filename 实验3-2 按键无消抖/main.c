/**
	* 特殊功能寄存器、I/O口地址
	* CT107D实验板的矩阵按键需要P4.2（0xC2）和P4.4 （0xC4）
	* 本项目是为了实现模拟电话按键（按键无效抖）
	* 实验现象：单次输入返回多次输出，无法正确交互
	**/

sfr  	 P0 = 0x80;
sfr 	 P2 = 0xA0;
sfr    P3 = 0xB0;
sfr	   P4 = 0xC0;

sbit P3_0 = 0xB0;
sbit P3_1 = 0xB1;
sbit P3_2 = 0xB2;
sbit P3_3 = 0xB3;
sbit P3_4 = 0xB4;
sbit P3_5 = 0xB5;
sbit P3_6 = 0xB6;
sbit P3_7 = 0xB7;

sbit P4_2 = 0xC2;
sbit P4_4 = 0xC4;

/**
	* EnableWrite 控制链表是否写入
	* row 全局变量 获取按下key的行坐标 
	* row 全局变量 获取按下key的列坐标
	* keyboard 使用二维数组 模拟键盘按键 
	* tube_label 控制八位数码管的选通
	* NULL 空指针 指向0x00
	*/

unsigned int EnableWrite = 0;
unsigned int row = 114;
unsigned int column = 514;
unsigned int keyboard[4][4] = {
							0xF9, 0xA4, 0xB0, 0xFF,
							0x99, 0x92, 0x82, 0xFF,
							0xF8, 0x80, 0x90, 0xFF,
							0xC0, 0xC0, 0xFF, 0xFF
															};

int tube_label[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

#define NULL 0x00;

/**
	* 定义循环链表(环)
	* 测试输入：0000 0000
	* 按键操作：1
	* 结果显示：0000 0001
	*/

typedef struct ring
{
	unsigned int Data;
	struct ring *Next;
}RING;

RING Node1;
RING Node2;
RING Node3;
RING Node4;
RING Node5;
RING Node6;
RING Node7;
RING Node8;

RING *ring_init(void)
{
	RING *head;
	Node1.Data = 0xFF;
	Node2.Data = 0xFF;
	Node3.Data = 0xFF;
	Node4.Data = 0xFF;
	Node5.Data = 0xFF;
	Node6.Data = 0xFF;
	Node7.Data = 0xFF;
	Node8.Data = 0xFF;
	
	Node1.Next = &Node2;
	Node2.Next = &Node3;
	Node3.Next = &Node4;
	Node4.Next = &Node5;
	Node5.Next = &Node6;
	Node6.Next = &Node7;
	Node7.Next = &Node8;
	Node8.Next = &Node1;
	
	head = &Node1;
	return head;
}

/**
	* 延时函数 为实现按键消抖，数码管消影
	*/
void delay(unsigned int ms)
{
	unsigned i,j;
	for(i=0; i<ms; i++)
	{
		for(j=0; j<120; j++);
	}
}

/**
	* 给链表写入数据
	* 返回的头指针是原先链表首节点的指针域
	* demo：	
	*						|-----------------------------|
	*           v															|
	*	创建节点 Node 1 -> Node -> Node 3 -> Node4.
	*						^
	*						|
	*          head
  *
  *	    		  |-----------------------------|
	*           v															|
  * 调用函数 Node 1 -> Node 2 -> Node 3 -> Node4.
	*						         ^
	*						         |
	*                   head
	*/

RING *write(RING *head, unsigned int written_Data)
{
	RING *pr = head;
	pr->Data = written_Data;
	return pr->Next;
}

/**
	* 在八位数码管上显示
	* 输入是循环链表（环）的头指针 
	*/

void display(RING *head)
{
	int loc;
	RING *pr = head;
		for(loc=0; loc<=7; loc++)
		{
			P0 = tube_label[loc];
			P2 = (P2 & 0x1F) | 0xC0;
			P2 &= 0x1F;
			
			P0 = pr->Data;
			P2 = (P2 & 0x1F) | 0xE0;
			P2 &= 0x1F;
			pr = pr->Next;
			delay(10);
		}
}

/**
	* 按键检测模块 结果更新row, column
	* goto 真好用
	*/

void get_key(void)
{

	P3 = (P3 & 0xC0) | 0x0F;
	P4 = (P4 & 0xEB) | 0x00;
	delay(0);
	if( P3_0 == 0 ) { row = 0; goto SEG1;}
	if( P3_1 == 0 ) { row = 1; goto SEG1;}
	if( P3_2 == 0 ) { row = 2; goto SEG1;}
	if( P3_3 == 0 ) { row = 3; goto SEG1;}
	return;
	SEG1:
	
	P3 = ( P3 & 0xC0) | 0x30;
	P4 = ( P4 & 0xEB) | 0x14;
	if( P3_4 == 0 ) { column = 3; goto SEG2;}
	if( P3_5 == 0 ) { column = 2; goto SEG2;}
	if( P4_2 == 0 ) { column = 1; goto SEG2;}
	if( P4_4 == 0 ) { column = 0; goto SEG2;}
	return;
	SEG2:
	{EnableWrite = 1; return;}
	
}


/**
	* 这里是按键识别和循环链表操作的耦合模块
	* 就是为了实现按什么键实现什么样的功能
	* 模拟电话按键及功能：
	* |-----------|
	* | 0000 0000 |
	*	|-----------|
	* |	1		2		3	|
	* |	4		5		6	|
	*	|	7		8		9	|
	* |	*		0		#	|
	*	|-----------|
  *
	* 按下* 其中_是熄灭
	* |-----------|
	* | ____ ___0 |
	*	|-----------|
	* |	1		2		3	|
	* |	4		5		6	|
	*	|	7		8		9	|
	* |	*		0		#	|
	*	|-----------|
	*
	* 按下# 				
	* |-----------|
	* | ____ ____ |
	*	|-----------|
	* |	1		2		3	|
	* |	4		5		6	|
	*	|	7		8		9	|
	* |	*		0		#	|
	*	|-----------|
	**/


RING *write_key(RING *head)
{
	RING *p = head;
	int j = 0;
	if(EnableWrite == 0) { return head; }
	
	if(row==3 && (column==0 || column==2))
	{
		for(j=1; j<=7; j++)
		{
			p->Data = 0xFF;
			p = p->Next;
		}
		if(column == 0) {p->Data = 0xC0; return head;}
		else {p->Data = 0xFF; return head;}
	}
	
	if(row<=3 && column<=3)
	{
		head = write(head, keyboard[row][column]);
		EnableWrite = 0;
	}
	return head;
}


void main()
{

	RING *head = 0x00;
	head = ring_init();
	
	P0 = 0xFF;
	P2 = (P2 & 0x1F) | 0x80;
	P2 &= 0x1F;
	
	while(1)
	{
		get_key();
		head = write_key(head);
		display(head);
	}
}