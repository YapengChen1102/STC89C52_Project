#include "CT107D_lib.h"

void main()
{

	RING *head = 0x00;
	head = ring_init();
	
	while(1)
	{
		get_key();
		head = write_key(head);
		display(head);
	}
}