/*===============================================================
 *                      Thread.cc
 *                     	内核线程 实现
 *                     							作者：柳暗花明-绿叶
 ===============================================================*/
#include "Thread.h"

using namespace Thread;
/************************************************************************/
/*							数据区
/************************************************************************/
BmpArray<KernelThread*,NR_THREAD> 	Thread::thread_addr_table;
KernelThread* 						Thread::run_thread;
DoubleLink<NR_THREAD>				Thread::ready_thread;
DoubleLink<NR_THREAD> 				Thread::sleep_thread;

/************************************************************************/
/*							创建线程
/*                          Create
/************************************************************************/
Result Thread::Create(ThreadFun fun,point params,u16 &ret_id)
{
	/* 分配一个线程描述符 */
	struct kernel_thread_desc new_thread;

	/* 向内存分配器申请一个页的空间 */
	KernelThread *new_space;
	/* 未完成 */

	/* 在线程地址表分配一个指针 */
	int id = 0;
	int result = thread_addr_table.Add(id,new_space);
	if (result != S_OK) return result;

	/* 初始化线程描述符 */
	new_thread.id = id;
	new_thread.state = THREAD_STATE_READY;
	new_thread.rec_des = 0;
	new_thread.save_fun = Thread::Save;
	new_thread.restart_fun = Thread::Restart;

	/* 并在就绪线程链表中添加一个项 */
	ready_thread.AddTail(id);
	ret_id = id;

	/* 线程描述符写入在线程空间中 */
	new_space->thread_info = new_thread;

	/* 函数地址和参数压入线程的栈 */
	new_space->thread_info.reg.eip = fun;

	/* 把esp的值设为栈顶并把参数压栈 */
	asm volatile("pushl %%edx			\n"
				"movl %%esp,%%edx		\n"		/* 保存内核栈esp */
				"movl %1,%%esp			\n"		/* esp指向线程栈顶 */
				"pushl %2				\n"		/* 参数入栈 */
				"movl %%esp,%0			\n"		/* esp保存到reg */
				"movl %%edx,%%esp		\n"		/* 恢复内核栈esp */
				"popl %%edx"
				:"+m"(new_space->thread_info.reg.esp)		/* %0,线程栈esp保存地址 */
				:"r"((&new_space)+1)						/* %1,线程栈顶 */
				 "r"(params)								/* %2,参数 */
				);

	return S_OK;
}

/************************************************************************/
/*							睡眠线程
/*                           Sleep
/************************************************************************/
Result Thread::Sleep(u16 id)
{
	KernelThread* thread;
	if(thread_addr_table.Get(id,thread)) return E_NOITEM;

	/* 如果处于运行态,保存上下文,（通过调用Save()函数） */
	if (run_thread->thread_info.id == id)
	{
		CALL_SAVE(run_thread->thread_info)
		run_thread = '\0';
		Schedule();
	}

	/* 把线程的状态设为睡眠态 */
	thread->thread_info.state = THREAD_STATE_SLEEP;

	/* 从就绪中移除 */
	if(ready_thread.Search(id))
		ready_thread.DeleteNext(id);

	/* 加入睡眠队列 */
	sleep_thread.AddTail(id);

	return S_OK;
}
/************************************************************************/
/*							唤醒线程
/*                           Wake
/************************************************************************/
Result Thread::Wake(u16 id)
{
	KernelThread* thread;
	if(thread_addr_table.Get(id,thread)) return E_NOITEM;

	/* 把线程的状态设为就绪态 */
	thread->thread_info.state = THREAD_STATE_READY;

	/* 从睡眠队列中移除 */
	sleep_thread.DeleteNext(id);

	/* 加入就绪队列 */
	ready_thread.AddTail(id);

	return S_OK;
}
/************************************************************************/
/*							杀死线程
/*                           Kill
/************************************************************************/
Result Thread::Kill(u16 id)
{
	/* 把线程从线程地址表中移除 */
	thread_addr_table.Delete(id);

	/* 把线程从任何一个队列中移除 */
	if(run_thread == obj)
	{
		run_thread = '\0';
	}
	else if(ready_thread.Search(id))
	{
		ready_thread.DeleteNext(id);
	}
	else if(sleep_thread.Search(id))
	{
		sleep_thread.DeleteNext(id);
	}

	/* 发送广播消息，通知各个部件撤销资源*/
	// SendMessage(MOD_FREE_SOURCE,&id);
	/* 未完成 */

	return S_OK;
}
