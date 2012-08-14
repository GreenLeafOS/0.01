/*
 * kernel.h
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include <lib/include/stack_array.h>
#include <lib/include/round_queue.h>
#include <lib/include/setbit.h>
#include <lib/include/list.h>

#include <arch/include/lock.h>
#include <arch/include/memory.h>
#include <arch/include/table.h>

#include "msg.h"
/************************************************************************/
/*							�ں˶���
/*							object
/************************************************************************/
// ģ��ͷ��Ϣ
typedef struct module_head
{
	FunAddr fun_msg_handle;
	FunAddr fun_init;
	u16 id;
}ModuleHead;



// ��Ϣͷ��Ϣ
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
	u16		body_size;
}MsgHead;
typedef void (*MsgHandle)(MsgHead msg_head);


// �㲥��Ϣע����
#define REG_TABLE_ITEM_MAX		16
#define REG_TABLE_MAX			128
typedef struct message_reg_item
{
	id_t mod_table[REG_TABLE_ITEM_MAX];
	u32 count;
}MsgRegItem;



// �߳�������
#define THREAD_STATE_RUNNING	1
#define THREAD_STATE_READY		2
#define THREAD_STATE_SLEEPING	4
typedef struct kernel_thread_desc
{
	ListNode node;	/* ���������̵߳Ľڵ� */
	id_t id;		/* �߳�ID */
	u16  flags;		/* ״̬��Ϣ */
	u16  priority;	/* ���ȼ� */
	u32  stack_top;	/* ջ�� */
	RoundQueueDefine(msg_queue,MsgHead,32,);	/* ��Ϣ���� */
}ThreadDesc;



// �߳�ջ
#define NR_THREAD 32
typedef union thread_union
{
	ThreadDesc thread_info;
	u32 stack[1024];		/* 4k��ջ */
}KernelThread;

/************************************************************************/
/*							�ں˺���
/*							function
/************************************************************************/
/* ģ�鴦���� */
int 	mod_load(ModuleHead* p_mod);

/* �̺߳��� */
id_t 	thread_create(ThreadFun fun,MsgHead msg_head);
int 	thread_sleep(id_t id);
int 	thread_wake(id_t id);
int 	thread_kill(id_t id);

void 	thread_schedule();

/* ��Ϣ���� */
id_t 	msg_send(MsgHead msg_head);
void	msg_handle(MsgHead msg_head);
int 	msg_register(id_t mod_id,u16 msg_type);

Bool 	msg_post(MsgHead msg_head);
MsgHead msg_recv();

/* �жϴ����� */
void	intr_handle(int irq_num);
/* �쳣������ */
void	exception_handler(int vec_num, int err_code);
/* ��ʼ���ж��쳣 */
void 	idt_init();
/* ��ຯ�� */
void 	save();
/************************************************************************/
/*							�ں�����
/*							data
/************************************************************************/
/* ģ��ע��� */
StackArrayDefine(mod_table,ModuleHead*,32,extern);

/* ��Ϣע��� */
extern MsgRegItem msg_reg_table[REG_TABLE_MAX];
extern StackArray msg_stack;

/* �߳� */
BmpArrayDefine(thread_table,KernelThread*,NR_THREAD,extern);

extern KernelThread*	thread_run;
extern u32*				thread_run_stack_top;
extern ListHead			thread_queue_ready;
extern ListHead			thread_queue_sleep;

/* �ں� */
extern int	kernel_reenter;

#endif /* KERNEL_H_ */
