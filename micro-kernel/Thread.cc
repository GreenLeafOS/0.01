/*===============================================================
 *                      Thread.cc
 *                     	内核线程 实现
 *                     							作者：柳暗花明-绿叶
 ===============================================================*/
#include "Thread.h"

using namespace Thread;
/*===============================================================
 *                        数据区
 ===============================================================*/
u32 			Thread::kernel_esp;
KernelThread* 	Thread::run_thread;
ThreadList 		Thread::ready_thread;
ThreadList 		Thread::sleep_thread;

/************************************************************************/
/*							创建线程
/*                          Create
/************************************************************************/
Result Thread::Create(ThreadFun fun,point params,u16 &ret_id)
{
	/* 分配一个线程描述符 */
	struct kernel_thread_desc new_thread;

	/* 向内存分配器申请一个页的空间 */
	/* 未完成 */
	KernelThread *new_space;

	/* 初始化线程描述符 */
	new_thread.state = THREAD_STATE_READY;
	new_thread.rec_des = 0;
	new_thread.save_fun = Thread::Save;
	new_thread.restart_fun = Thread::Restart;

	/* 并在就绪线程链表中添加一个项 */
	ready_thread.AddHead(new_space);

	/* 初始化栈 */
	new_thread.id = ready_thread.head;
	ret_id = ready_thread.head;
	new_space->thread_info = new_thread;

	/* 函数地址和参数压入线程的栈 */
	new_space->thread_info.reg.eip = fun;
	/* 把esp的值设为栈顶并把参数压栈 */
	asm volatile("pushl %%edx			\n"		/* 保存内核栈esp */
				"movl %%esp,%%edx		\n"
				"movl %1,%%esp			\n"		/* esp指向线程栈顶 */
				"pushl %2				\n"		/* 参数入栈 */
				"movl %%esp,%0			\n"		/* esp保存到reg */
				"movl %%edx,%%esp		\n"		/* 恢复内核栈esp */
				"popl %%edx"
				:"+m"(new_space->thread_info.reg.esp)		/* %0,线程栈esp保存地址 */
				:"r"((&new_space)+1)						/* %1,线程栈顶 */
				 "r"(params)								/* %2,参数 */
				);
	return;
}

/************************************************************************/
/*							杀死线程
/*                           Kill
/************************************************************************/
Result Thread::Kill(u16 id)
{
	KernelThread* obj;
	KernelThread* tmp;

	ready_thread.Get(id,obj);

	/* 把线程从任何一个队列中移除 */
	while(run_thread == obj)
	{
		run_thread = '\0';
	}
l_ready:
	while(ready_thread.Get(tmp) == S_OK)
	{
		if (tmp == obj)
		{
			ready_thread.Delete();
			goto l_recovery;
		}
		if (ready_thread.MoveNext() != S_OK)
		{
			goto l_sleep;
		}
	}
l_sleep:
	while(sleep_thread.Get(tmp) == S_OK)
	{
		if (tmp == obj)
		{
			sleep_thread.Delete();
			goto l_recovery;
		}
		if (sleep_thread.MoveNext() != S_OK)
		{
			goto l_recovery;
		}
	}
l_recovery:

	/* 发送广播消息，通知各个部件撤销资源*/
	/* 未完成 */
	return S_OK;
}
