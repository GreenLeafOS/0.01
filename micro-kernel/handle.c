/*
 * handle.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/sysapi.h"
#include "include/handle.h"

/* handle data */
int	handle_err_code;
int handle_ret_addr;




/*
 * handle intr
 * ������
 * 		irq_num	�жϺ�
 * ���ܣ��жϴ���
 * ����ֵ����Ӧrestart�ĵ�ַ
 * */
void handle_intr(int irq_num)
{
	MsgHead msg = {MSG_INTR + irq_num,
			msg_priority_table[MSG_INTR + irq_num],
			0,
			0,
			NULL,
			0};
//	post(msg);
	thread_schedule();
	return;
}




/*
 * handle exception
 * ������
 * 		vec_num �쳣������
 * 		err_code ������
 * ���ܣ��쳣����
 * ����ֵ����
 * */
void handler_exception(int vec_num, int err_code)
{
	MsgHead msg = {MSG_EXCP + vec_num,
				msg_priority_table[MSG_EXCP + vec_num],
				0,
				0,
				&err_code,
				sizeof(int)};
	post(msg);
	return;
}
