#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	内核线程 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "DataStruct.h"
#include "Lock.h"

/*===========线程上下文寄存器结构=============*/
typedef struct stack_frame {
	/* 单个压入 */
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	/* pushal压入*/
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	/* 系统压入 */
	u32	eip;
	u32	cs;
	u32	eflags;
	u32	esp;
	u32	ss;
}StackFrame;
#define REGFRAME_HIGH 	&Thread::run_thread->thread_info.reg.eax
#define REGFRAME_LOW 	&Thread::run_thread->thread_info.reg.gs

/*==============线程描述符================*/
struct kernel_thread_desc
{
	u16 id;			/* 线程ID */
	u16 state;		/* 状态信息 */
	u32 rec_des;	/* 占有资源描述符 */
	FunAddr save_fun;		/* 专有的保存状态的函数 */
	FunAddr restart_fun;	/* 专有的恢复状态的函数 */
	StackFrame reg;	/* 寄存器值 */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4


/*==============线程栈==================*/
typedef union thread_union
{
	struct kernel_thread_desc thread_info;
	u32 stack[1024];		/* 4k的栈 */
}KernelThread;
#define NR_THREAD 32



/*==============Thread命名空间==================*/
namespace Thread
{
/*************************线程状态控制函数**********************************/
	/* 创建线程
	 * 分配一个线程描述符
	 * 初始化线程描述符
	 * 并在就绪线程链表中添加一个项
	 * 初始化栈
	 * 函数地址和参数压入线程的栈 */
	Result Create(ThreadFun fun,point params,u16 &ret_id);

	/* 睡眠线程
	 * 如果处于运行态,保存上下文,（通过调用Save()函数）
	 * 把线程的状态设为睡眠态
	 * 从就绪或运行队列中移除
	 * 加入睡眠队列 */
	Result Sleep(u16 id);

	/* 唤醒线程
	 * 把线程的状态设为就绪态
	 * 从睡眠队列中移除
	 * 加入就绪队列 */
	Result Wake(u16 id);

	/* 杀死线程
	 * 把线程从任何一个队列中移除
	 * 释放线程占有的任何资源(发送广播消息) */
	Result Kill(u16 id);

	/* 设置线程的自定义恢复，保存函数
	 * 这种机制用于支持涉及优先级转换的线程 */
	inline Result SetFun(int id,FunAddr save,FunAddr restart);

/***************************仅针对于run_thread***************************/
/****************缺省函数，在没有制定save_fun,和restart_fun的时候。**********/

	/* 保存run_thread的通用寄存器和部分段寄存器到线程栈 */
	void Save();
	/* 恢复并跳到线程中执行 */
	void Restart();


	/* 调度算法,选择一个线程作为run_thread */
	void Schedule();

/*******************************数据区***********************************/
	typedef ShareLinkList<KernelThread*,NR_THREAD> ThreadList;
	typedef BmpArray<struct link_item<KernelThread*>,NR_THREAD> StaticData;
	extern u32 kernel_esp;
	extern KernelThread* run_thread;
	extern ThreadList ready_thread;
	extern ThreadList sleep_thread;
};

/************************************************************************/
/*					设置线程的自定义恢复，保存函数
/*                          SetFun
/************************************************************************/
inline Result Thread::SetFun(int id,FunAddr save,FunAddr restart)
{
	KernelThread*  *p_thread;

	/* 获取线程指针 */
	if (ready_thread.GetAddr(id,&p_thread) != S_OK) return E_NOITEM;

	/* 设置函数 */
	LOCK()
		p_thread->thread_info.save_fun = save;
		p_thread->thread_info.restart_fun = restart;
	UNLOCK()

	return S_OK;
}


#endif /* KERNEL_H_ */
