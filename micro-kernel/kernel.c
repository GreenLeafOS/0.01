/*
 * Kernel.cc
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */


int	kernel_reenter;


void KernelMain()
{
	__asm("nop");
	while(1);
}





