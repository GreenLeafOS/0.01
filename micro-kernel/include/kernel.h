/*
 * kernel.h
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <lib/include/stack_array.h>
#include <arch/include/lock.h>
/************************************************************************/
/*							内核对象
/*							object
/************************************************************************/
// 模块头信息
typedef struct module_head
{
	FunAddr fun_msg_handle;
	FunAddr fun_init;
	u16 id;
}ModuleHead;



// 消息头信息
#define MSG_PRIORITY_REALTIME	0
#define MSG_PRIORITY_KERNEL		1
#define MSG_PRIORITY_DRIVER		2
#define MSG_PRIORITY_USER		3
typedef struct message_head
{
	u16		type;
	u16		priority;
	id_t	sender;
	id_t	receiver;
	point	body_point;
}MsgHead;
typedef void (*MsgHandle)(MsgHead msg_head);


// 广播消息注册项
#define REG_TABLE_ITEM_MAX		16
#define REG_TABLE_MAX			128
typedef struct message_reg_item
{
	id_t mod_table[REG_TABLE_ITEM_MAX];
	u32 count;
}MsgRegItem;



// 线程描述符
#define THREAD_STATE_RUNNING	1
#define THREAD_STATE_READY		2
#define THREAD_STATE_SLEEPING	4
typedef struct kernel_thread_desc
{
	id_t id;		/* 线程ID */
	u16  flags;		/* 状态信息 */
	u16  priority;	/* 优先级 */
	u32  stack_top;	/* 栈顶 */
	/* 消息队列 */
}ThreadDesc;



// 线程栈
#define NR_THREAD 32
typedef union thread_union
{
	ThreadDesc thread_info;
	u32 stack[1024];		/* 4k的栈 */
}KernelThread;

/************************************************************************/
/*							内核函数
/*							function
/************************************************************************/
/* 模块处理函数 */
int 	mod_load(ModuleHead* p_mod);

/* 线程函数 */
id_t 	thread_create(ThreadFun fun,point params);
int 	thread_sleep(id_t id);
int 	thread_wake(id_t id);
int 	thread_kill(id_t id);

void 	thread_chedule();

/* 消息函数 */
id_t 	msg_send(MsgHead msg_head);
void	msg_handle(MsgHead msg_head);
int 	msg_register(id_t mod_id,u16 msg_type);

Bool 	msg_post(MsgHead msg_head);
MsgHead msg_recv();


/************************************************************************/
/*							内核数据
/*							data
/************************************************************************/
/* 模块注册表 */
StackArrayDefine(mod_table,ModuleHead*,32,extern);

/* 消息注册表 */
extern MsgRegItem msg_reg_table[REG_TABLE_MAX];
extern StackArray msg_stack;

/* 线程 */

#endif /* KERNEL_H_ */
