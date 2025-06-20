#ifndef __CT107D_Lib_H__
#define __CT107D_Lib_H__

// 数据结构是循环链表
typedef struct ring
{
	unsigned int Data;
	struct ring *Next;
}RING;

// 外部变量
extern unsigned int EnableWrite;
extern unsigned int row;
extern unsigned int column;

// 函数声明
void delay(unsigned int ms);
struct ring *ring_init(void);
struct ring *write(RING *head, unsigned int written_Data);
void display(RING *head);
void get_key();
RING *write_key(RING *head);


#endif