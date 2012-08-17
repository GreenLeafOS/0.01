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
 * ��������Ϣͷ
 * ���ܣ�������Ϣ��ֱ�ӷ���
 * ����ֵ����
 */
void post(MsgHead msg)
{
	KernelLock();
		/* ��ȡ��������Ϣ */
		KernelThread* thread = thread_table[msg.receiver];
		/* ������Ϣ�����еĿ������� */
		int id = bmp_search(&thread->thread_info.msg_queue_bmp,THREAD_NR_MSGQUEUE);

		if (thread == NULL || id == -1) return;

		/* д����Ϣ */
		bmp_set(&thread->thread_info.msg_queue_bmp,id);
		thread->thread_info.msg_queue[id] = msg;

		/* ���ѽ�����Ϣ���߳� */
		if (thread->thread_info.state == THREAD_STATE_RECVING)
			wake(thread);
	KernelUnlock();
	return;
}




/*
 * SysApi recv
 * ��������
 * ���ܣ�������Ϣ�����û����Ϣ��˯���Լ�
 * ����ֵ����Ϣͷ
 */
MsgHead recv()
{
	KernelLock();
		int priority = 10;
		int id;

		/* ѭ���ȴ�������Ϣ */
		while(thread_run->thread_info.msg_queue_bmp == 0)
		{
			thread_run->thread_info.state == THREAD_STATE_RECVING;
			wait();
		}


		/* ѡ�����ȼ���ߵ���Ϣ */
		for(int i=0;i<THREAD_NR_MSGQUEUE;i++)
		{
			if(bmp_test((void*)&thread_run->thread_info.msg_queue_bmp,i))
			{
				/* ��ȡ��Ϣָ�� */
				MsgHead *msg = &thread_run->thread_info.msg_queue[i];

				/* �����Ϣ�����ȼ������ */
				if(msg->priority == 0)
				{
					bmp_clear((void*)&thread_run->thread_info.msg_queue_bmp,i);
					return *msg;
				}

				/* ѡ��ϸ����ȼ�����Ϣ */
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

