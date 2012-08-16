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
KernelThread*	create();						/* �����߳� */
void			ready(KernelThread* thread);	/* �̳߳�ʼ����ϣ�������� */
void			sleep(KernelThread* thread);	/* ˯���߳� */
void			wake(KernelThread* thread);		/* �����߳� */
void			kill(KernelThread* thread);		/* ɱ���߳� */
/* thread.s */
void			wait();							/* �߳�����˯�ߣ��Եȴ�ĳ���¼� */
void			exit();							/* �߳��˳� */
/* message.c */
void			post(MsgHead msg);				/* ������Ϣ������ȴ��� */
MsgHead			recv();							/* ������Ϣ */
/* lock.c */
void			lock();							/* ��ֹ�ں���ռ */
void			unlock();						/* �����ں���ռ */
/* phypage.c */
PhyPage			alloc();						/* ��������ҳ�� */
void			free(PhyPage* page);			/* �ͷ�����ҳ�� */

#endif /* SYSAPI_H_ */
