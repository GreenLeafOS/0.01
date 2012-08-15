/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#include "include/kernel.h"

int	kernel_reenter;		// �ж�Ƕ����ȡ�-1��ʾ�̣߳�0��ʾ��������������ʾǶ����ȡ�
int *kernel_stack_p;	// �ں�ջ�������ַ


void main_thread_create();


void KernelMain()
{
	char *str = "\n\nIn Kernel now.\n";
	print(str);

	kernel_stack_p = &tss.esp0;

	main_thread_create();
	kernel_reenter = 0;

	restart();

	while(1);
}


void main_thread()
{
	char *str = "main_thread\n";
	print(str);

	char ch[] = {'m','\0'};
	while(1)
	{
		for(int i=0;i<100;i++)
			for(int j=0;j<100;j++)
				for(int k=0;k<100;k++);
		print(&ch);
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
	regs.eip = (u32)main_thread;

	regs.eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	id_t id = thread_fork(regs);

	thread_run = thread_table[id];
	thread_run_stack_top = &thread_table[id]->thread_info.stack_top;
}



