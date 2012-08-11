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


	/* 把esp的值设为栈顶并把参数压栈 */
	__asm volatile(
				"pushl %%ebp			\n"
				"movl %%esp,%%ebp		\n"		/* 保存内核栈esp */
				"movl (%0),%%esp		\n"		/* esp指向线程栈顶 */
				"pushl %1				\n"		/* 返回地址入栈 */
				"pushl %2				\n"		/* 参数：消息头入栈 */
				"movl %%esp,(%0)		\n"		/* esp保存 */
				"movl %%ebp,%%esp		\n"		/* 恢复内核栈esp */
				"popl %%ebp"
				:
				:"g"(&new_space->thread_info.stack_top),	/* %0,线程栈esp保存地址 */
				 "g"(fun),									/* %1,返回地址 */
				 "g"(msg_head)								/* %2,消息头参数 */
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
