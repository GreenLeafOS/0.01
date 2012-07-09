#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	内核线程 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "DataStruct.h"


/*==============线程描述符================*/
struct kernel_thread
{
	u16 id;			/* 线程ID */
	u16 state;		/* 状态信息 */
	u32 rec_des;	/* 占有资源描述符 */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4

/*==============线程栈==================*/
typedef union thread_union
{
	struct kernel_thread thread_info;
	u32 stack[2048];		/* 8k的栈 */
}Thread;
#define NR_THREAD 32

/*==============Thread命名空间==================*/
namespace Thread
{
/*************************线程状态控制函数**********************************/
	/* 创建线程
	 * 分配一个线程描述符
	 * 初始化线程描述符
	 * 并在就绪线程链表中添加一个项
	 * 函数地址和参数压入线程的栈
	 */
	Result Create(ThreadFun fun,point params);

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


/***************************仅针对于run_thread***************************/

	/* 保存run_thread的上下文 */
	void Save();

	/* 恢复run_thread的上下文，并把控制权交给线程 */
	void Restart();

	/* 调度算法,选择一个线程作为run_thread */
	void Schedule();

/*******************************数据区***********************************/
	typedef ShareLinkList<Thread*,NR_THREAD> ThreadList;

	Thread* run_thread;
	ThreadList ready_thread;
	ThreadList sleep_thread;
}


#endif /* KERNEL_H_ */
