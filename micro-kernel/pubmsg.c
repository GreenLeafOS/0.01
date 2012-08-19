/*
 * pubmsg.c
 *
 *  Created on: 2012-8-17
 *      Author: greenleaf
 */


#include "include/sysapi.h"
#include "include/lib.h"

#define NR_MSG_REG		16



/* msg_reg_item */
struct msg_reg_item
{
	id_t thread_table[NR_MSG_REG];
	u32	 count;
}msg_reg_table[128] = {0};




/*
 * 消息处理
 */
void public_msg_do(MsgHead msg)
{
	if (msg.vector == MSG_THREAD_REGPUB)
	{
		/* 注册消息 */
		struct msg_reg_item* item = &msg_reg_table[msg.param];
		if (item->count == (NR_MSG_REG - 1))
		{
			__asm(".global debug2\ndebug2:\n");
			/* 表项已满 */
			msg_max.sender = 1;
			msg_max.receiver = msg.sender;
			post(msg_max);			// 发送答复：没有表项了。
			return;
		}
		else
		{
			__asm(".global debug\ndebug:\n");
			/* 注册 */
			item->thread_table[item->count++] = msg.sender;

			msg_ok.sender = 1;
			msg_ok.receiver = msg.sender;
			post(msg_ok);			// 发送答复：处理成功
			return;
		}
	}
	else
	{
		__asm(".global debug1\ndebug1:\n");
		/* 转发消息 */
		u32	count = msg_reg_table[msg.vector].count;
		for (int i=0;i<count;i++)
		{
			msg.receiver = msg_reg_table[msg.vector].thread_table[i];
			post(msg);

		}
	}
}



/*
 * 主循环
 */
void public_msg_main()
{
	char *str = "public_msg_main.\n";
	print(str);
	while(1)
	{
		MsgHead msg = recv();		// 获取消息
		public_msg_do(msg);			// 处理消息
		for(int i=0;i<1000000;i++);
		char *ch = "S";
		print(ch);
	}
}




