/*
 * handle.c
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */


#include "include/kernel.h"



/*
 * ��ʼ���ж��쳣
 * */
void idt_kernel_init()
{

}



/*
 * �жϴ�����
 * ������
 * 		irq_num	�жϺ�
 * ����ֵ����Ӧrestart�ĵ�ַ
 * */
FunAddr intr_handle(int irq_num)
{
	return restart;
}


/*
 * �쳣������
 * ������
 * 		vec_num �쳣������
 * 		err_code ������
 * ����ֵ����Ӧrestart�ĵ�ַ
 * */
FunAddr exception_handler(int vec_num, int err_code)
{
	return restart;
}
