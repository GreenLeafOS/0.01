/*
 * message.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/sysapi.h"

/* message data */
MsgHead msg_ok = {MSG_RET_OK,MSG_PRIORITY_RET};
MsgHead msg_max = {MSG_RET_MAX,MSG_PRIORITY_RET};

/*
 * SysApi post
 * 参数：消息头
 * 功能：发送消息，直接返回
 * 返回值：无
 */
void post(MsgHead msg)
{
	KernelLock();
		/* 获取接收者信息 */
		KernelThread* thread = thread_table[msg.receiver];
		/* 搜索消息队列中的空闲区域 */
		int id = bmp_search(&thread->thread_info.msg_queue_bmp,THREAD_NR_MSGQUEUE);

		if (thread == NULL || id == -1) return;

		/* 写入信息 */
		bmp_set(&thread->thread_info.msg_queue_bmp,id);
		thread->thread_info.msg_queue[id] = msg;

		/* 唤醒接收消息的线程 */
		if (thread->thread_info.state == THREAD_STATE_RECVING)
			wake(thread);
	KernelUnlock();
	return;
}




/*
 * SysApi recv
 * 参数：无
 * 功能：接受消息，如果没有消息则睡眠自己
 * 返回值：消息头
 */
MsgHead recv()
{
	KernelLock();
		int priority = 10;
		int id;

		/* 循环等待接收消息 */
		while(thread_run->thread_info.msg_queue_bmp == 0)
		{
			thread_run->thread_info.state == THREAD_STATE_RECVING;
			wait();
		}


		/* 选择优先级最高的消息 */
		for(int i=0;i<THREAD_NR_MSGQUEUE;i++)
		{
			if(bmp_test((void*)&thread_run->thread_info.msg_queue_bmp,i))
			{
				/* 获取消息指针 */
				MsgHead *msg = &thread_run->thread_info.msg_queue[i];

				/* 如果消息的优先级已最高 */
				if(msg->priority == 0)
				{
					bmp_clear((void*)&thread_run->thread_info.msg_queue_bmp,i);
					return *msg;
				}

				/* 选择较高优先级的消息 */
				if(msg->priority < priority)
				{
					id = i;
					priority = msg->priority;
				}
			}
		}

		bmp_clear((void*)&thread_run->thread_info.msg_queue_bmp,id);
	KernelUnlock();

	return thread_run->thread_info.msg_queue[id];
}

