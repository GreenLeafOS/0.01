/*
 * message.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#include "include/kernel.h"


/* ��Ϣע��� */
MsgRegItem msg_reg_table[REG_TABLE_MAX];
StackArray msg_stack = {REG_TABLE_ITEM_MAX,sizeof(id_t),0,(u8*)&msg_reg_table};


/*
 * ������Ϣ
 */
id_t msg_send(MsgHead msg_head)
{
	if(msg_head.priority == MSG_PRIORITY_REALTIME)
	{
		KernelLock();			// ���ж�
		msg_handle(msg_head);	// ������Ϣ
		KernelUnlock();			// ���ж�
	}
	else
	{
		// thread_create
	}
	return -1;
}



/*
 * �����ش�����Ϣ
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
 * ע��㲥��Ϣ
 */
int msg_register(id_t mod_id,u16 msg_type)
{
	msg_stack.count = msg_reg_table[msg_type].count;
	int id = stack_array_add(&msg_stack,&mod_id);
	return id;
}




/*
 * ������Ϣ
 */
MsgHead msg_recv()
{

}




// ������Ϣ
Bool msg_post(MsgHead msg_head)
{

}






