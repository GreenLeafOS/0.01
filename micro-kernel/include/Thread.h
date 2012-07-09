#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	�ں��߳� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "DataStruct.h"
#include "Type.h"
#include "Atomic.h"

/* �߳������� */
struct kernel_thread
{
	u16 id;			/* �߳�ID */
	u32 mem_des;	/* �ڴ������� */
	u32 state;		/* ״̬��Ϣ */
};

/* �߳�ջ */
typedef union thread_union
{
	struct kernel_thread thread_info;
	u32 stack[2048];		/* 8k��ջ */
};


namespace Thread
{
	/* �����߳�
	 * ��ʼ���߳�������
	 * ���ھ����߳����������һ����
	 * ������ַ�Ͳ���ѹ���̵߳�ջ
	 */
	Result Create(ThreadFun fun,point params);

	Result Sleep(u16 id);
};


#endif /* KERNEL_H_ */
