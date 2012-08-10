/*
 * 8259A.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */


#include "include/8259A.h"

/*======================================================================*
                            init_8259A
 *======================================================================*/
void init_8259A()
{
	outb(INT_M_CTL,	0x11);					// Master 8259, ICW1. 00010001
	outb(INT_S_CTL,	0x11);					// Slave  8259, ICW1. 00010001

	outb(INT_M_CTLMASK,	INT_VECTOR_IRQ0);	// Master 8259, ICW2. 0x20 ��8259���ж�����
	outb(INT_S_CTLMASK,	INT_VECTOR_IRQ8);	// Slave  8259, ICW2. 0x28 ��8259���ж�����

	outb(INT_M_CTLMASK,	0x4);				// Master 8259, ICW3. 00000100	IR2������Ƭ
	outb(INT_S_CTLMASK,	0x2);				// Slave  8259, ICW3. 0x2       ��Ƭ������Ƭ��IR��

	outb(INT_M_CTLMASK,	0x1);				// Master 8259, ICW4. 00000001	8086ģʽ,����EOI
	outb(INT_S_CTLMASK,	0x1);				// Slave  8259, ICW4. 00000001

	outb(INT_M_CTLMASK,	0xFF);				// Master 8259, OCW1. 11111111
	outb(INT_S_CTLMASK,	0xFF);				// Slave  8259, OCW1. 11111111
}