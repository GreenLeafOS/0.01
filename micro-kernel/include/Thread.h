#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	内核线程 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "DataStruct.h"
#include "Lock.h"
#include "reg.h"

/*==============线程描述符================*/
struct kernel_thread_desc
{
	u16 		id;				/* 线程ID */
	u16 		state;			/* 状态信息 */
	u32 		rec_des;		/* 占有资源描述符 */
	FunAddr 	save_fun;		/* 专有的保存状态的函数 */
	FunAddr 	restart_fun;	/* 专有的恢复状态的函数 */
	StackFrame 	reg;			/* 寄存器值 */
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
/***********线程状态控制函数************/
	/* 创建线程
	 * 分配一个线程描述符
	 * 初始化线程描述符
	 * 并在就绪线程链表中添加一个项
	 * 初始化栈
	 * 函数地址和参数压入线程的栈
	 */
	Result Create(ThreadFun fun,point params,u16 &ret_id);

	/* 睡眠线程
	 * 如果处于运行态,保存上下文,（通过调用Save()函数）
	 * 把线程的状态设为睡眠态
	 * 从就绪或运行队列中移除
	 * 加入睡眠队列
	 */
	Result Sleep(u16 id);

	/* 唤醒线程
	 * 把线程的状态设为就绪态
	 * 从睡眠队列中移除
	 * 加入就绪队列
	 */
	Result Wake(u16 id);

	/* 杀死线程
	 * 把线程从任何一个队列中移除
	 * 释放线程占有的任何资源(发送广播消息)
	 */
	Result Kill(u16 id);

	/* 设置线程的自定义恢复，保存函数
	 * 这种机制用于支持涉及优先级转换的线程
	 */
	inline Result SetFun(int id,FunAddr save,FunAddr restart);

/******仅针对于run_thread*************/
	#define CALL_SAVE(desc)		\
			(*desc->save_fun);
	#define CALL_RESTART(desc)	\
			(*desc->restart_fun);

	/* 保存run_thread的通用寄存器和部分段寄存器到线程栈
	 */
	void Save();

	/* 恢复并跳到线程中执行
	 */
	void Restart();


	/* 调度算法,选择一个线程作为run_thread
	 */
	void Schedule();

/**************数据区***************/
	extern BmpArray<KernelThread*,NR_THREAD> thread_addr_table;
	extern KernelThread* run_thread;
	extern DoubleLink<NR_THREAD> ready_thread;
	extern DoubleLink<NR_THREAD> sleep_thread;
};

/************************************************************************/
/*					设置线程的自定义恢复，保存函数
/*                          SetFun
/************************************************************************/
inline Result Thread::SetFun(int id,FunAddr save,FunAddr restart)
{
	KernelThread *p_thread;

	/* 获取线程指针 */
	if (thread_addr_table.Get(id,p_thread) != S_OK) return E_NOITEM;

	/* 设置函数 */
	LOCK()
		p_thread->thread_info.save_fun = save;
		p_thread->thread_info.restart_fun = restart;
	UNLOCK()

	return S_OK;
}


#endif /* KERNEL_H_ */
