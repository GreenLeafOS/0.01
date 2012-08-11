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
	new_thread.stack_top = new_space->stack[1023];	// 栈顶


	/* 并在就绪线程链表中添加一个项 */
	// list_add(&thread_ready,&new_space->thread_info.node);


	/* 线程描述符写入在线程空间中 */
	new_space->thread_info = new_thread;


	__asm volatile(
			/* 把esp的值设为线程栈顶 */
			"pushl %%ebp			\n"
			"movl %%esp,%%ebp		\n"		/* 保存内核栈esp */
			"movl (%0),%%esp		\n"		/* esp指向线程栈顶 */

			/* 参数入栈 */
			"subl %3,%%esp			\n"		/* 分配消息头在栈中的空间 */

			"movl %%esp,%%edi		\n"		/* 目的指针 */
			"movl %1,%%esi			\n"		/* 源指针 */
			"movl $3,%%ecx			\n"		/* 计数器 */
			"rep;movsb				\n"		/* 重复传送 */

			"pushl %2				\n"		/* 返回地址入栈 */

			/* 保存线程esp，恢复内核esp */
			"movl %%esp,(%0)		\n"		/* esp保存 */
			"movl %%ebp,%%esp		\n"		/* 恢复内核栈esp */
			"popl %%ebp"

			::"g"(&new_space->thread_info.stack_top),	/* %0,线程栈esp保存地址 */
			 "g"(&msg_head),				/* %1,消息头指针 */
			 "g"(fun),						/* %2,返回地址 */
			 "g"(sizeof(msg_head))			/* %3,消息头大小 */
			 );

	return id;
}


int thread_sleep(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* 如果处于运行态,保存上下文,（通过调用Save()函数） */
	if (thread_run->thread_info.id == id)
	{
		// Save
		thread_run = NULL;
		thread_schedule();
	}

	/* 把线程的状态设为睡眠态 */
	thread->thread_info.flags = THREAD_STATE_SLEEPING;

	/* 从就绪队列移除 */
//	list_delete(&thread_queue_ready,&thread);

	/* 加入睡眠队列 */
//	list_add(&thread_queue_sleep,&thread);

}


int thread_wake(id_t id)
{
	KernelThread* thread;
	if (!(BITTEST(thread_table_data,id))) return E_NOITEM;
	thread = thread_table[id];

	/* 把线程的状态设为就绪态 */
	thread->thread_info.flags = THREAD_STATE_READY;

	/* 从睡眠队列中移除 */
//	list_delete(&thread_queue_sleep,&thread);

	/* 加入就绪队列 */
//	list_add(&thread_queue_ready,&thread);

}


int thread_kill(id_t id)
{
	/* 把线程从线程地址表中移除 */
	// thread_addr_table.Delete(id);

	/* 把线程从任何一个队列中移除 */

	/* 发送广播消息，通知各个部件撤销资源*/
	// SendMessage(MOD_FREE_SOURCE,&id);
	/* 未完成 */
}

void thread_schedule()
{

}
