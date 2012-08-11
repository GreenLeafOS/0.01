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


	__asm volatile(
			/* ��esp��ֵ��Ϊ�߳�ջ�� */
			"pushl %%ebp			\n"
			"movl %%esp,%%ebp		\n"		/* �����ں�ջesp */
			"movl (%0),%%esp		\n"		/* espָ���߳�ջ�� */

			/* ������ջ */
			"subl %3,%%esp			\n"		/* ������Ϣͷ��ջ�еĿռ� */

			"movl %%esp,%%edi		\n"		/* Ŀ��ָ�� */
			"movl %1,%%esi			\n"		/* Դָ�� */
			"movl $3,%%ecx			\n"		/* ������ */
			"rep;movsb				\n"		/* �ظ����� */

			"pushl %2				\n"		/* ���ص�ַ��ջ */

			/* �����߳�esp���ָ��ں�esp */
			"movl %%esp,(%0)		\n"		/* esp���� */
			"movl %%ebp,%%esp		\n"		/* �ָ��ں�ջesp */
			"popl %%ebp"

			::"g"(&new_space->thread_info.stack_top),	/* %0,�߳�ջesp�����ַ */
			 "g"(&msg_head),				/* %1,��Ϣͷָ�� */
			 "g"(fun),						/* %2,���ص�ַ */
			 "g"(sizeof(msg_head))			/* %3,��Ϣͷ��С */
			 );

	return id;
}


int thread_sleep(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* �����������̬,����������,��ͨ������Save()������ */
	if (thread_run->thread_info.id == id)
	{
		// Save
		thread_run = NULL;
		thread_schedule();
	}

	/* ���̵߳�״̬��Ϊ˯��̬ */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* �Ӿ��������Ƴ� */
//	list_delete(&thread_queue_ready,&thread);

	/* ����˯�߶��� */
//	list_add(&thread_queue_sleep,&thread);

}


int thread_wake(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* ���̵߳�״̬��Ϊ����̬ */
	thread->thread_info.flags = THREAD_STATE_READY;

	/* ��˯�߶������Ƴ� */
//	list_delete(&thread_queue_sleep,&thread);

	/* ����������� */
//	list_add(&thread_queue_ready,&thread);

}


int thread_kill(id_t id)
{
	/* ���̴߳��̵߳�ַ�����Ƴ� */
	// thread_addr_table.Delete(id);

	/* ���̴߳��κ�һ���������Ƴ� */

	/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
	// SendMessage(MOD_FREE_SOURCE,&id);
	/* δ��� */
}

void thread_schedule()
{

}
