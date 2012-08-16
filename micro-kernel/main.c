/*
 * main.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/main.h"
#include "include/sysapi.h"
#include "include/handle.h"


/* main data */
u32* kernel_stack_top;
int kernel_reenter;


void main_thread_create();

void kernel_main()
{
	char *str = "\n\nGreenLeafOS version 0.01.\n";
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
 * 主线程
 */
void kernel_main_thread()
{
	char* str = "main thread.\n";
	print(str);
	while(1)
	{
		for (int i=0;i<100000;i++);
		char *str = "M";
		print(str);
	}
	while(1);
}


void testA()
{
	for(int i=0;i<5;i++)
	{
		for (int i=0;i<1000000;i++);
		char *str = "A";
		print(str);
	}
	while(1);
//	exit();
}

void testB()
{
	for(int i=0;i<2;i++)
	{
		for (int i=0;i<1000000;i++);
		char *str = "B";
		print(str);
	}
	while(1);
//	wait();
}

void testC()
{
	while(1)
	{
		for (int i=0;i<1000000;i++);
		char *str = "C";
		print(str);
	}
}

void main_thread_create()
{
	StackFrame regs;
	u16 code = gdt_get_sel(KERNEL_CODE,0);
	u16 data = gdt_get_sel(KERNEL_DATA,0);
	u16 video = gdt_get_sel(USER_VIDEO,0);
	KernelThread* thread;

	regs.ds = data;
	regs.es = data;
	regs.fs = data;
	regs.gs = video;

	regs.ss = data;

	regs.cs = code;
	regs.eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	/* 建立线程 */
	regs.eip = (u32)kernel_main_thread;
	thread = create();
	thread->thread_info.stack_top -= sizeof(regs);
	*(StackFrame*)thread->thread_info.stack_top = regs;
	ready(thread);


	regs.eip = (u32)testA;
	thread = create();
	thread->thread_info.stack_top -= sizeof(regs);
	*(StackFrame*)thread->thread_info.stack_top = regs;
	ready(thread);


	regs.eip = (u32)testB;
	thread = create();
	thread->thread_info.stack_top -= sizeof(regs);
	*(StackFrame*)thread->thread_info.stack_top = regs;
	ready(thread);



	regs.eip = (u32)testC;
	thread = create();
	thread->thread_info.stack_top -= sizeof(regs);
	*(StackFrame*)thread->thread_info.stack_top = regs;
	ready(thread);

}
