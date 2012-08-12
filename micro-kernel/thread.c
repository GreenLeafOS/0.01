/*
 * thread.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */


#include "include/kernel.h"


BmpArrayDefine(thread_table,KernelThread*,NR_THREAD,);

KernelThread*	thread_run;
ListHead		thread_queue_ready;
ListHead		thread_queue_sleep;

/*
 * �����߳�
 */
id_t thread_create(ThreadFun fun,MsgHead msg_head)
{
	/* ����һ���߳������� */
	struct kernel_thread_desc new_thread;


	/* ���ڴ����������һ��ҳ�Ŀռ� */
	KernelThread *new_space;
	new_space = (KernelThread *)mem_page_alloc();


	/* ���̵߳�ַ�����һ��ָ�� */
	int id = bmp_search(&thread_table_data,NR_THREAD);
	if (id != -1)
	{
		thread_table[id] = new_space;		// д��ָ��
		bmp_set(&thread_table_data,id);		// ����λͼ
	}
	else
	{
		return -1;
	}


	/* ��ʼ���߳������� */
	new_thread.id = id;
	new_thread.flags = THREAD_STATE_READY;
	new_thread.priority = msg_head.priority;		// ��ʱ�̳���Ϣ�����ȼ�
	new_thread.stack_top = (u32)(((u8*)(&new_space->stack[1023])) - msg_head.body_size);	// ջ��(��������Ϣ��Ŀռ�)

	/* �߳�������д�����߳̿ռ��� */
	new_space->thread_info = new_thread;

	/* �ھ����߳����������һ���� */
	list_add(&thread_queue_ready,&new_space->thread_info.node);


	/* ��Ϣ����ջ������Ϣ��ָ��ָ��ջ������ */
	msg_head.body_point = memcpy(
			(void*)new_space->thread_info.stack_top,	// dst
			(void*)msg_head.body_point,				// src
			msg_head.body_size);				// size

	/* ��Ϣͷ��ջ */
	new_space->thread_info.stack_top - sizeof(msg_head);
	memcpy((void*)new_space->thread_info.stack_top,	// dst
			(void*)&msg_head,							// src
			sizeof(msg_head));					// size

	/* ���ص�ַ��ջ */
	new_space->thread_info.stack_top - sizeof(fun);
	memcpy((void*)new_space->thread_info.stack_top,	// dst
			(void*)&fun,								// src
			sizeof(fun));						// size

	return id;
}


int thread_sleep(id_t id)
{
	KernelThread* thread;
	if (!(bmp_test(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* �����������̬,����������,��ͨ������Save()������ */
	if (thread_run->thread_info.id == id)
	{
		// Save();
		thread_run = NULL;
		thread_schedule();
	}

	/* ���̵߳�״̬��Ϊ˯��̬ */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* �Ӿ��������Ƴ� */
	list_unlink(&thread->thread_info.node);

	/* ����˯�߶��� */
	list_add(&thread_queue_sleep,&thread->thread_info.node);

}


int thread_wake(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* ���̵߳�״̬��Ϊ����̬ */
	thread->thread_info.flags = THREAD_STATE_READY;

	/* ��˯�߶������Ƴ� */
	list_unlink(&thread->thread_info.node);

	/* ����������� */
	list_add(&thread_queue_ready,&thread->thread_info.node);

}


int thread_kill(id_t id)
{
	/* ���̴߳��̵߳�ַ�����Ƴ� */
	bmp_clear(&thread_table_data,id);

	/* ���̴߳��κ�һ���������Ƴ� */
	list_unlink(&thread_table[id]->thread_info.node);

	/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
	MsgHead msg = {MSG_THREAD_KILL,	// type
			MSG_PRIORITY_REALTIME,	// priority
			id,						// sender
			0,						// receiver
			NULL,					// body_point
			0};						// body_size

	msg_handle(msg);
}

void thread_schedule()
{

}
