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
 * 根据消息创建线程
 */
id_t thread_create(FunAddr fun,MsgHead msg_head,int cpl)
{
	/* 分配一个线程描述符 */
	struct kernel_thread_desc new_thread;


	/* 向内存分配器申请一个页的空间 */
	KernelThread *new_space;
	new_space = (KernelThread *)mem_page_alloc();


	/* 在线程地址表分配一个指针 */
	int id = bmp_search(&thread_table_data,NR_THREAD);
	if (id != -1)
	{
		thread_table[id] = new_space;		// 写入指针
		bmp_set(&thread_table_data,id);		// 设置位图
	}
	else
	{
		return -1;
	}


	/* 初始化线程描述符 */
	new_thread.id = id;
	new_thread.flags = THREAD_STATE_READY;
	new_thread.priority = msg_head.priority;		// 暂时继承消息的优先级
	new_thread.stack_top = (u32)(((u8*)(&new_space->stack[4096])) - msg_head.body_size);	// 栈顶(开辟了消息体的空间)
	new_thread.ticks = (5-new_thread.priority)* 400;

	/* 线程描述符写入在线程空间中 */
	new_space->thread_info = new_thread;

	/* 在就绪线程链表中添加一个项 */
	list_add(&thread_queue_ready,&new_space->thread_info.node);

	/* 初始化堆栈数据 */
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



	/* 消息体入栈并将消息体指针指向栈中数据 */
	msg_head.body_point = memcpy(
			(void*)new_space->thread_info.stack_top,	// dst
			(void*)msg_head.body_point,					// src
			msg_head.body_size);						// size

	/* 消息头入栈 */
	new_space->thread_info.stack_top -=  sizeof(msg_head);
	memcpy((void*)new_space->thread_info.stack_top,		// dst
			(void*)&msg_head,							// src
			sizeof(msg_head));							// size



	/* 寄存器入栈 */
	new_space->thread_info.stack_top -= sizeof(*regs);
	regs = (StackFrame*)(new_space->thread_info.stack_top);

	regs->ds = data;
	regs->es = data;
	regs->fs = data;
	regs->gs = data;

	regs->ss = data;
	regs->esp =(u32)new_space->thread_info.stack_top;	// 最低地址

	regs->cs = code;
	regs->eip = (u32)fun;

	regs->eflags = 0x1202;	// IF = 1,IOPL = 1, bit 2 is always 1.

	return id;
}


/*
 * 创建线程
 */
id_t thread_fork(StackFrame regs)
{
	/* 分配一个线程描述符 */
	struct kernel_thread_desc new_thread;


	/* 向内存分配器申请一个页的空间 */
	KernelThread *new_space;
	new_space = (KernelThread *)mem_page_alloc();


	/* 在线程地址表分配一个指针 */
	int id = bmp_search(&thread_table_data,NR_THREAD);
	if (id != -1)
	{
		thread_table[id] = new_space;		// 写入指针
		bmp_set(&thread_table_data,id);		// 设置位图
	}
	else
	{
		return -1;
	}


	/* 初始化线程描述符 */
	new_thread.id = id;
	new_thread.flags = THREAD_STATE_READY;
	new_thread.priority = 1;
	new_thread.stack_top = (u32)&new_space->stack[4096]  - sizeof(regs);	// 栈顶
	new_thread.ticks = (5-new_thread.priority)* 400;

	/* 线程描述符写入在线程空间中 */
	new_space->thread_info = new_thread;

	/* 在就绪线程链表中添加一个项 */
	list_add(&thread_queue_ready,&new_space->thread_info.node);

	StackFrame* dst_regs = (StackFrame*)(new_space->thread_info.stack_top);
	regs.esp = (u32)new_space->thread_info.stack_top;
	*dst_regs = regs;

	return id;
}

int thread_sleep(KernelThread* thread)
{
	if (!(bmp_test(thread_table_data,thread->thread_info.id))) return E_NOITEM;

	/* 把线程的状态设为睡眠态 */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* 从就绪队列移除 */
	list_unlink(&thread->thread_info.node);

	/* 加入睡眠队列 */
	list_add(&thread_queue_sleep,&thread->thread_info.node);

	return 1;
}


int thread_wake(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* 把线程的状态设为就绪态 */
	thread->thread_info.flags = THREAD_STATE_READY;

	/* 从睡眠队列中移除 */
	list_unlink(&thread->thread_info.node);

	/* 加入就绪队列 */
	list_add(&thread_queue_ready,&thread->thread_info.node);

}


int thread_kill(id_t id)
{
	/* 把线程从线程地址表中移除 */
	bmp_clear(&thread_table_data,id);

	/* 把线程从任何一个队列中移除 */
	list_unlink(&thread_table[id]->thread_info.node);

	/* 发送广播消息，通知各个部件撤销资源*/
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
