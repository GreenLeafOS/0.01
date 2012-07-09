#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	内核线程 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "DataStruct.h"
#include "Type.h"
#include "Atomic.h"

/* 线程描述符 */
struct kernel_thread
{
	u16 id;			/* 线程ID */
	u32 mem_des;	/* 内存描述符 */
	u32 state;		/* 状态信息 */
};

/* 线程栈 */
typedef union thread_union
{
	struct kernel_thread thread_info;
	u32 stack[2048];		/* 8k的栈 */
};


namespace Thread
{
	/* 创建线程
	 * 初始化线程描述符
	 * 并在就绪线程链表中添加一个项
	 * 函数地址和参数压入线程的栈
	 */
	Result Create(ThreadFun fun,point params);

	Result Sleep(u16 id);
};


#endif /* KERNEL_H_ */
