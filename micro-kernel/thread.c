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
 * 创建线程
 */
id_t thread_create(ThreadFun fun,MsgHead msg_head)
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
	new_thread.stack_top = (u32)(((u8*)(&new_space->stack[1023])) - msg_head.body_size);	// 栈顶(开辟了消息体的空间)

	/* 线程描述符写入在线程空间中 */
	new_space->thread_info = new_thread;

	/* 在就绪线程链表中添加一个项 */
	list_add(&thread_queue_ready,&new_space->thread_info.node);


	/* 消息体入栈并将消息体指针指向栈中数据 */
	msg_head.body_point = memcpy(
			(void*)new_space->thread_info.stack_top,	// dst
			(void*)msg_head.body_point,				// src
			msg_head.body_size);				// size

	/* 消息头入栈 */
	new_space->thread_info.stack_top - sizeof(msg_head);
	memcpy((void*)new_space->thread_info.stack_top,	// dst
			(void*)&msg_head,							// src
			sizeof(msg_head));					// size

	/* 返回地址入栈 */
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

	/* 如果处于运行态,保存上下文,（通过调用Save()函数） */
	if (thread_run->thread_info.id == id)
	{
		// Save();
		thread_run = NULL;
		thread_schedule();
	}

	/* 把线程的状态设为睡眠态 */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* 从就绪队列移除 */
	list_unlink(&thread->thread_info.node);

	/* 加入睡眠队列 */
	list_add(&thread_queue_sleep,&thread->thread_info.node);

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

}
