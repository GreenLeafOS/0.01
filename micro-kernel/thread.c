/*
 * thread.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */


#include "include/kernel.h"
#include "include/msg.h"

BmpArrayDefine(thread_table,KernelThread*,NR_THREAD,);

KernelThread*	thread_run;
ListHead		thread_queue_ready;
ListHead		thread_queue_sleep;

int				thread_sleep_id;
u32* 			thread_run_stack_top;

/*
 * ������Ϣ�����߳�
 */
id_t thread_create(FunAddr fun,MsgHead msg_head,int cpl)
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
	new_thread.stack_top = (u32)(((u8*)(&new_space->stack[4096])) - msg_head.body_size);	// ջ��(��������Ϣ��Ŀռ�)
	new_thread.ticks = (5-new_thread.priority)* 400;

	/* �߳�������д�����߳̿ռ��� */
	new_space->thread_info = new_thread;

	/* �ھ����߳����������һ���� */
	list_add(&thread_queue_ready,&new_space->thread_info.node);

	/* ��ʼ����ջ���� */
	StackFrame *regs;
	u16 code,data;

	if (cpl == 0)
	{
		code = gdt_get_sel(KERNEL_CODE,0);
		data = gdt_get_sel(KERNEL_DATA,0);
	}
	else if (cpl == 3)
	{
		code = gdt_get_sel(USER_CODE,3);
		data = gdt_get_sel(USER_DATA,3);
	}
	else
	{
		return -2;
	}



	/* ��Ϣ����ջ������Ϣ��ָ��ָ��ջ������ */
	msg_head.body_point = memcpy(
			(void*)new_space->thread_info.stack_top,	// dst
			(void*)msg_head.body_point,					// src
			msg_head.body_size);						// size

	/* ��Ϣͷ��ջ */
	new_space->thread_info.stack_top -=  sizeof(msg_head);
	memcpy((void*)new_space->thread_info.stack_top,		// dst
			(void*)&msg_head,							// src
			sizeof(msg_head));							// size



	/* �Ĵ�����ջ */
	new_space->thread_info.stack_top -= sizeof(*regs);
	regs = (StackFrame*)(new_space->thread_info.stack_top);

	regs->ds = data;
	regs->es = data;
	regs->fs = data;
	regs->gs = data;

	regs->ss = data;
	regs->esp =(u32)new_space->thread_info.stack_top;	// ��͵�ַ

	regs->cs = code;
	regs->eip = (u32)fun;

	regs->eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	return id;
}


/*
 * �����߳�
 */
id_t thread_fork(StackFrame regs)
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
	new_thread.priority = 1;
	new_thread.stack_top = (u32)&new_space->stack[4096]  - sizeof(regs);	// ջ��
	new_thread.ticks = (5-new_thread.priority)* 400;

	/* �߳�������д�����߳̿ռ��� */
	new_space->thread_info = new_thread;

	/* �ھ����߳����������һ���� */
	list_add(&thread_queue_ready,&new_space->thread_info.node);

	StackFrame* dst_regs = (StackFrame*)(new_space->thread_info.stack_top);
	regs.esp = (u32)new_space->thread_info.stack_top;
	*dst_regs = regs;

	return id;
}

int thread_sleep(KernelThread* thread)
{
	if (!(bmp_test(thread_table_data,thread->thread_info.id))) return E_NOITEM;

	/* ���̵߳�״̬��Ϊ˯��̬ */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* �Ӿ��������Ƴ� */
	list_unlink(&thread->thread_info.node);

	/* ����˯�߶��� */
	list_add(&thread_queue_sleep,&thread->thread_info.node);

	return 1;
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
	while(1)
	{
		int i = 0;
		KernelThread *thread =(KernelThread*)list_search(&thread_queue_ready,i++);
		if (thread == NULL) return;
		if (thread->thread_info.ticks-- != 0)
		{
			thread_run = thread;
			thread_run_stack_top =(u32*)&thread->thread_info.stack_top;
			return;
		}
		else
		{
			thread_run->thread_info.ticks = (5-thread_run->thread_info.priority)* 200;
		}
	}
}
