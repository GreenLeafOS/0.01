/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */


int	kernel_reenter;		// �ж�Ƕ����ȡ�-1��ʾ�̣߳�0��ʾ��������������ʾǶ����ȡ�
int kernel_stack;

void KernelMain()
{
	kernel_reenter = -1;
	__asm("cli");
	while(1);
}





