/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#include "include/kernel.h"

int	kernel_reenter;		// 中断嵌入深度。-1表示线程，0表示正常处理。正数表示嵌套深度。
int *kernel_stack_p;	// 内核栈顶保存地址


void main_thread_create();


void KernelMain()
{
	list_init(&thread_queue_ready);
	list_init(&thread_queue_sleep);

	char *str = "\n\nIn Kernel now.\n";
	print(str);

	kernel_stack_p = &tss.esp0;

	kernel_reenter = 0;
	main_thread_create();


	restart();

	while(1);
}


void main_thread()
{
	char *str = "main_thread\n";
	print(str);
	while(1)
	{
		for (int i=0;i<1000000;i++);
		char *str = "M";
		print(str);
	}
	while(1);
}

void testA()
{
	while(1)
	{
		for (int i=0;i<1000000;i++);
		char *str = "A";
		print(str);
	}
}

void testB()
{
	while(1)
	{
		for (int i=0;i<1000000;i++);
		char *str = "B";
		print(str);

	}
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

	regs.ds = data;
	regs.es = data;
	regs.fs = data;
	regs.gs = video;

	regs.ss = data;

	regs.cs = code;
	regs.eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	/* 建立线程 */
	regs.eip = (u32)main_thread;
	thread_fork(regs);

	regs.eip = (u32)testA;
	thread_fork(regs);
	regs.eip = (u32)testB;
	thread_fork(regs);
	regs.eip = (u32)testC;
	thread_fork(regs);

	thread_run = thread_table[0];
	thread_run_stack_top =(u32*)&thread_run->thread_info.stack_top;
}



