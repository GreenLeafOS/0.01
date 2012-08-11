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
	new_thread.stack_top = new_space->stack[1023];	// ջ��


	/* ���ھ����߳����������һ���� */
	// list_add(&thread_ready,&new_space->thread_info.node);


	/* �߳�������д�����߳̿ռ��� */
	new_space->thread_info = new_thread;


	/* ��esp��ֵ��Ϊջ�����Ѳ���ѹջ */
	__asm volatile(
				"pushl %%ebp			\n"
				"movl %%esp,%%ebp		\n"		/* �����ں�ջesp */
				"movl (%0),%%esp		\n"		/* espָ���߳�ջ�� */
				"pushl %1				\n"		/* ���ص�ַ��ջ */
				"pushl %2				\n"		/* ��������Ϣͷ��ջ */
				"movl %%esp,(%0)		\n"		/* esp���� */
				"movl %%ebp,%%esp		\n"		/* �ָ��ں�ջesp */
				"popl %%ebp"
				:
				:"g"(&new_space->thread_info.stack_top),	/* %0,�߳�ջesp�����ַ */
				 "g"(fun),									/* %1,���ص�ַ */
				 "g"(msg_head)								/* %2,��Ϣͷ���� */
				);

	return id;
}


int thread_sleep(id_t id)
{

}


int thread_wake(id_t id)
{

}


int thread_kill(id_t id)
{

}

void thread_schedule()
{

}
