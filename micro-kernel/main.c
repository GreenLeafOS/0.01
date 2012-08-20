/*
 * main.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/main.h"
#include "include/sysapi.h"
#include "include/irq.h"
#include "include/module.h"

/* main data */
u32* kernel_stack_top;
int kernel_reenter;



/*
 * �ں�������
 */
void kernel_main()
{
	char *str = "GreenLeafOS version 0.01.\n";
	print(str);

	kernel_stack_top = (u32*)&tss.esp0;
	kernel_reenter = 0;
	thread_init();

	main_thread_create();

	thread_schedule();
	restart();

	while(1);
}





/*
 * kernel_main_thread
 * ���߳�
 */
void kernel_main_thread()
{
	while(1);
}






/*
 * main_thread_create
 * �����߳�
 */
void main_thread_create()
{
	KernelThread* thread;

	/* �����߳� */
	CreateThread(mod_public_msg,	thread,		thread_default_reg);
	CreateThread(mod_time,			thread,		thread_default_reg);
}
