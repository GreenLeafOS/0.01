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
	kernel_stack_p = &tss.esp0;

	__asm("cli");
	while(1);
}





