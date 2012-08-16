/*
 * thread.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/sysapi.h"

/* thread data */
KernelThread*	thread_table[NR_THREAD];
KernelThread*	thread_run;
u32* 			thread_run_stack_top;
ListHead		thread_queue_ready;
ListHead		thread_queue_sleep;




/*
 * thread init
 * ��������
 * ���ܣ���ʼ���̹߳�������
 * ����ֵ����
 */
void thread_init()
{
	memset(&thread_table[0],0,sizeof(thread_table));
	thread_run = thread_table[0];
	thread_run_stack_top =&thread_run->thread_info.stack_top;
	list_init(&thread_queue_ready);
	list_init(&thread_queue_sleep);
}




/*
 * thread set
 * �������߳�ָ��
 * ���ܣ����߳�ָ��д���̱߳�
 * ����ֵ���ɹ�����id,ʧ�ܷ���-1
 */
id_t thread_set(KernelThread* thread)
{
	for(int i=0;i<NR_THREAD;i++)
	{
		/* ��Чָ����ΪNULL */
		if (thread_table[i] == NULL)
		{
			thread_table[i] = thread;
			return i;
		}
	}
	return -1;
}




/*
 * thread wait
 * ��������
 * ���ܣ�wait�ĺ�������
 * ����ֵ����
 */
void thread_wait()
{
	/* �Ӿ��������Ƴ� */
	list_unlink(&thread_run->thread_info.node);

	/* ����˯�߶��� */
	list_add(&thread_queue_sleep,&thread_run->thread_info.node);

	/* ���õ��ȳ��� */
	thread_schedule();
}




/*
 * thread exit
 * ��������
 * ���ܣ�exit�ĺ�������
 * ����ֵ����
 */
void thread_exit()
{
	id_t id = thread_run->thread_info.id;

	/* ���̴߳��̵߳�ַ�����Ƴ� */
	thread_table[id] = NULL;

	/* ���̴߳��κ�һ���������Ƴ� */
	list_unlink(&thread_run->thread_info.node);

	/* ����״̬ */
	thread_run->thread_info.state = THREAD_STATE_EXIT;

	/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
	MsgHead msg = {MSG_THREAD_KILL,	// vector
			MSG_PRIORITY_REALTIME,	// priority
			id,						// sender
			0,						// receiver
			NULL,					// body_point
			0};						// body_size
	post(msg);
}


/*
 * thread schedule
 * ��������
 * ���ܣ������߳�
 * ����ֵ����
 */
void thread_schedule()
{
	if (thread_run == NULL)
	{
		thread_run = thread_table[0];
		thread_run->thread_info.state = THREAD_STATE_RUNNING;
		thread_run_stack_top =(u32*)&thread_run->thread_info.stack_top;
	}

	// ����Ƿ���ں���ռ if ()

	/* ��������߳��Ƿ���Ҫ�л� */
	if (thread_run->thread_info.state == THREAD_STATE_RUNNING)
	{
		if (thread_run->thread_info.ticks--)
		{
			return;
		}
		/* ʱ��Ƭ���꣬���� */
		else
		{
			thread_run->thread_info.ticks = (5-thread_run->thread_info.priority)* 2;
			thread_run->thread_info.state = THREAD_STATE_READY;
			list_addtail(&thread_queue_ready,&thread_run->thread_info.node);
		}
	}

	/* ������ִ���߳� */
	int i = 0;
	while(1)
	{
		KernelThread *thread =(KernelThread*)list_search(&thread_queue_ready,i++);
		if (thread == NULL)
		{
			thread_run = thread_table[0];
			thread_run->thread_info.state = THREAD_STATE_RUNNING;
			list_unlink(&thread_run->thread_info.node);
			thread_run_stack_top =(u32*)&thread_run->thread_info.stack_top;
			return;
		}
		if (thread->thread_info.id == 0)
		{
			continue;
		}
		if (thread->thread_info.ticks)
		{
			thread_run = thread;
			thread_run->thread_info.state = THREAD_STATE_RUNNING;
			thread_run_stack_top =(u32*)&thread_run->thread_info.stack_top;

			list_unlink(&thread_run->thread_info.node);
			return;
		}
	}
}




/*
 * SysApi create
 * ��������
 * ���ܣ������߳�
 * ����ֵ���߳�ָ��
 */
KernelThread* create()
{
	KernelLock();
		/* ���ڴ����������һ��ҳ�Ŀռ� */
		KernelThread *new_space = (KernelThread*)mem_page_alloc();
		if (new_space == NULL) return NULL;

		/* �����߳�ָ��д���̱߳���ȡһ������id */
		id_t id = thread_set(new_space);
		if (id == -1) return NULL;

		/* ��ʼ���߳������� */
		new_space->thread_info.id = id;
		new_space->thread_info.state = THREAD_STATE_START;		// ����̬
		new_space->thread_info.priority = 2;
		new_space->thread_info.stack_top = (u32)&new_space->stack[8192];	// ջ��
		new_space->thread_info.ticks = (5-new_space->thread_info.priority)* 4;
	KernelUnlock();

	return new_space;
}



/*
 * SysApi ready
 * �������߳�ָ��
 * ���ܣ��̼߳������
 * ����ֵ����
 */
void ready(KernelThread* thread)
{
	KernelLock();
		if (thread == NULL) return;

		/* ׼��̬ */
		thread->thread_info.state = THREAD_STATE_READY;

		/* ����������� */
		list_add(&thread_queue_ready,&thread->thread_info.node);
	KernelUnlock();
}




/*
 * SysApi sleep
 * �������߳�ָ��
 * ���ܣ�˯���߳�
 * ����ֵ����
 */
void sleep(KernelThread* thread)
{
	KernelLock();
		if (thread == NULL) return;

		/* ˯��̬ */
		thread->thread_info.state = THREAD_STATE_SLEEPING;

		/* �Ӿ��������Ƴ� */
		list_unlink(&thread->thread_info.node);

		/* ����˯�߶��� */
		list_add(&thread_queue_sleep,&thread->thread_info.node);
	KernelUnlock();
}




/*
 * SysApi wake
 * �������߳�ָ��
 * ���ܣ������߳�
 * ����ֵ����
 */
void wake(KernelThread* thread)
{
	KernelLock();
		if (thread == NULL) return;

		/* ����̬ */
		thread->thread_info.state = THREAD_STATE_READY;

		/* ��˯�߶������Ƴ� */
		list_unlink(&thread->thread_info.node);

		/* ����������� */
		list_add(&thread_queue_ready,&thread->thread_info.node);
	KernelUnlock();
}




/*
 * SysApi kill
 * �������߳�ָ��
 * ���ܣ�ɱ���߳�
 * ����ֵ����
 */
void kill(KernelThread* thread)
{
	KernelLock();
		id_t id = thread->thread_info.id;

		/* ���̴߳��̵߳�ַ�����Ƴ� */
		thread_table[id] = NULL;

		/* ���̴߳��κ�һ���������Ƴ� */
		list_unlink(&thread->thread_info.node);

		/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
		MsgHead msg = {MSG_THREAD_KILL,	// vector
				MSG_PRIORITY_REALTIME,	// priority
				id,						// sender
				0,						// receiver
				NULL,					// body_point
				0};						// body_size
		post(msg);
	KernelUnlock();
}
