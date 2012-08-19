/*
 * main.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/main.h"
#include "include/sysapi.h"
#include "include/handle.h"
#include "include/module.h"

/* main data */
u32* kernel_stack_top;
int kernel_reenter;


void main_thread_create();
void public_msg_main();

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
 * ���߳�
 */
void kernel_main_thread()
{
	char* str = "main thread.\n";
	print(str);
	while(1)
	{
		for(int i=0;i<1000000;i++);
		char *ch = "M";
		print(ch);
	}
}


void main_thread_create()
{
	KernelThread* thread;
	StackFrame regs;
	u16 code = gdt_get_sel(KERNEL_CODE,0);
	u16 data = gdt_get_sel(KERNEL_DATA,0);
	u16 video = gdt_get_sel(USER_VIDEO,0);


	regs.ds = data;
	regs.es = data;
	regs.fs = data;
	regs.gs = video;

	regs.ss = data;

	regs.cs = code;
	regs.eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	/* �����߳� */
	regs.eip = (u32)kernel_main_thread;
	CreateThread(thread,regs);
	thread->thread_info.ticks = 0;

	regs.eip = (u32)public_msg_main;
	CreateThread(thread,regs);
	thread->thread_info.priority = 0;

	regs.eip = (u32)mod_time_main;
	CreateThread(thread,regs);
	mod_time_id = thread->thread_info.id;
}
