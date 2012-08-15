/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#include "include/kernel.h"

int	kernel_reenter;		// �ж�Ƕ����ȡ�-1��ʾ�̣߳�0��ʾ��������������ʾǶ����ȡ�
int *kernel_stack_p;	// �ں�ջ�������ַ

void KernelMain()
{
	kernel_reenter = -1;
	kernel_reenter = 0;

	tss.ss0 = gdt_get_sel(1,0);
	tss.esp0 = 0x80000;

	kernel_stack_p = &tss.esp0;

	char *str = "\n\nIn Kernel now.\n";
	print(str);

	__asm(	"movl	$0x124e,%eax		\n"
			"movl	$0x646,%ebx			\n"
			"movl	$0x8455,%ecx		\n"
			"movl	$0x983d,%edx		\n"	);

	__asm("sti");
//	__asm("cli");
	while(1);
}





