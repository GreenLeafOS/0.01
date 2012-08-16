/*
 * sysapi.h
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#ifndef SYSAPI_H_
#define SYSAPI_H_

#include "thread.h"
#include "message.h"
#include "phypage.h"

/* thread.c */
KernelThread*	create();						/* 创建线程 */
void			ready(KernelThread* thread);	/* 线程初始化完毕，加入调度 */
void			sleep(KernelThread* thread);	/* 睡眠线程 */
void			wake(KernelThread* thread);		/* 唤醒线程 */
void			kill(KernelThread* thread);		/* 杀死线程 */
/* thread.s */
void			wait();							/* 线程自我睡眠，以等待某个事件 */
void			exit();							/* 线程退出 */
/* message.c */
void			post(MsgHead msg);				/* 发送消息（无需等待） */
MsgHead			recv();							/* 接收消息 */
/* lock.c */
void			lock();							/* 禁止内核抢占 */
void			unlock();						/* 允许内核抢占 */
/* phypage.c */
PhyPage			alloc();						/* 分配物理页框 */
void			free(PhyPage* page);			/* 释放物理页框 */

#endif /* SYSAPI_H_ */
