/*
 * thread.h
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#ifndef THREAD_H_
#define THREAD_H_
#include "lib.h"
#include "message.h"


/* ThreadDesc */
typedef struct kernel_thread_desc
{
	ListNode node;					/* ���������̵߳Ľڵ� */
	id_t id;						/* �߳�ID */
	u16  state;						/* ״̬��Ϣ */
	u16  priority;					/* ���ȼ� */
	u16  ticks;						/* ʱ��Ƭ */
	u32  stack_top;					/* ջ�� */
	MsgHead msg_queue[32];			/* ��Ϣ���� */
	u32 msg_queue_bmp;				/* ��Ϣ����λͼ */
}ThreadDesc;


/* ThreadDesc.state */
#define THREAD_STATE_RUNNING	1
#define THREAD_STATE_READY		2
#define THREAD_STATE_SLEEPING	4
#define THREAD_STATE_SENDING	8
#define THREAD_STATE_RECVING	16
#define THREAD_STATE_START		32
#define THREAD_STATE_EXIT		64


/* KernelThread */
typedef union thread_union
{
	ThreadDesc thread_info;
	u8 stack[8192];		/* 8k��ջ */
}KernelThread;


/* KernelThread[] */
#define NR_THREAD 32



/* thread function */
void thread_init();
id_t thread_set(KernelThread* thread);
void thread_wait();
void thread_exit();
void thread_schedule();

/* thread data */
extern KernelThread*	thread_table[NR_THREAD];
extern KernelThread*	thread_run;
extern u32*				thread_run_stack_top;
extern ListHead			thread_queue_ready;
extern ListHead			thread_queue_sleep;


#endif /* THREAD_H_ */
