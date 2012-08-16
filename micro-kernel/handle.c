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
 * 参数：
 * 		irq_num	中断号
 * 功能：中断处理
 * 返回值：对应restart的地址
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
 * 参数：
 * 		vec_num 异常向量号
 * 		err_code 错误码
 * 功能：异常处理
 * 返回值：无
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
