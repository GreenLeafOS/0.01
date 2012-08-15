/*
 * message.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#include "include/kernel.h"


/* 消息注册表 */
MsgRegItem msg_reg_table[REG_TABLE_MAX];
StackArray msg_stack = {REG_TABLE_ITEM_MAX,sizeof(id_t),0,(u8*)&msg_reg_table};


/*
 * 发送消息
 * msg_head.receiver 0 表示广播消息
 */
id_t msg_send(MsgHead msg_head)
{
	if(msg_head.priority == MSG_PRIORITY_REALTIME)
	{
		KernelLock();			// 关中断

		if (msg_head.type == 0)
			msg_handle(msg_head);	// 处理广播消息
		else
			(*((MsgHandle)mod_table[msg_head.receiver]->fun_msg_handle))(msg_head);

		KernelUnlock();			// 开中断
	}
	else
	{
		if (msg_head.type == 0)
			thread_create(msg_handle,msg_head,0);
		else
			thread_create((FunAddr)mod_table[msg_head.receiver]->fun_msg_handle,msg_head,0);
	}
	return -1;
}



/*
 * 完整地处理消息
 */
void msg_handle(MsgHead msg_head)
{
	for(int i=0;i<msg_reg_table[msg_head.type].count;i++)
	{
		id_t mod_id = msg_reg_table[msg_head.type].mod_table[i];
		(*((MsgHandle)mod_table[mod_id]->fun_msg_handle))(msg_head);
	}
}



/*
 * 注册广播消息
 */
int msg_register(id_t mod_id,u16 msg_type)
{
	msg_stack.count = msg_reg_table[msg_type].count;
	int id = stack_array_add(&msg_stack,&mod_id);
	return id;
}




/*
 * 接收消息
 */
MsgHead msg_recv()
{
	MsgHead ret = {0};
	while(1)
	{
		MsgHead *p =(MsgHead*)round_queue_delete(
				&thread_table[thread_run->thread_info.id]->
				thread_info.msg_queue_data);
		if (p == NULL)
		{
			thread_sleep_self();
		}
		else
		{
			ret = *p;
			return ret;
		}
	}
	return ret;
}




// 发送消息
Bool msg_post(MsgHead msg_head)
{
	Result ret = round_queue_add(
			&thread_table[msg_head.receiver]->
			thread_info.msg_queue_data,
			msg_head);

	if (ret == E_MAX) return False;
	return True;
}






