/*===============================================================
 *                      Thread.cc
 *                     	�ں��߳� ʵ��
 *                     							���ߣ���������-��Ҷ
 ===============================================================*/
#include "Thread.h"

using namespace Thread;
/*===============================================================
 *                        ������
 ===============================================================*/
u32 			Thread::kernel_esp;
KernelThread* 	Thread::run_thread;
ThreadList 		Thread::ready_thread;
ThreadList 		Thread::sleep_thread;

/************************************************************************/
/*							�����߳�
/*                          Create
/************************************************************************/
Result Thread::Create(ThreadFun fun,point params,u16 &ret_id)
{
	/* ����һ���߳������� */
	struct kernel_thread_desc new_thread;

	/* ���ڴ����������һ��ҳ�Ŀռ� */
	/* δ��� */
	KernelThread *new_space;

	/* ��ʼ���߳������� */
	new_thread.state = THREAD_STATE_READY;
	new_thread.rec_des = 0;
	new_thread.save_fun = Thread::Save;
	new_thread.restart_fun = Thread::Restart;

	/* ���ھ����߳����������һ���� */
	ready_thread.AddHead(new_space);

	/* ��ʼ��ջ */
	new_thread.id = ready_thread.head;
	ret_id = ready_thread.head;
	new_space->thread_info = new_thread;

	/* ������ַ�Ͳ���ѹ���̵߳�ջ */
	new_space->thread_info.reg.eip = fun;
	/* ��esp��ֵ��Ϊջ�����Ѳ���ѹջ */
	asm volatile("pushl %%edx			\n"		/* �����ں�ջesp */
				"movl %%esp,%%edx		\n"
				"movl %1,%%esp			\n"		/* espָ���߳�ջ�� */
				"pushl %2				\n"		/* ������ջ */
				"movl %%esp,%0			\n"		/* esp���浽reg */
				"movl %%edx,%%esp		\n"		/* �ָ��ں�ջesp */
				"popl %%edx"
				:"+m"(new_space->thread_info.reg.esp)		/* %0,�߳�ջesp�����ַ */
				:"r"((&new_space)+1)						/* %1,�߳�ջ�� */
				 "r"(params)								/* %2,���� */
				);
	return;
}

/************************************************************************/
/*							ɱ���߳�
/*                           Kill
/************************************************************************/
Result Thread::Kill(u16 id)
{
	KernelThread* obj;
	KernelThread* tmp;

	ready_thread.Get(id,obj);

	/* ���̴߳��κ�һ���������Ƴ� */
	while(run_thread == obj)
	{
		run_thread = '\0';
	}
l_ready:
	while(ready_thread.Get(tmp) == S_OK)
	{
		if (tmp == obj)
		{
			ready_thread.Delete();
			goto l_recovery;
		}
		if (ready_thread.MoveNext() != S_OK)
		{
			goto l_sleep;
		}
	}
l_sleep:
	while(sleep_thread.Get(tmp) == S_OK)
	{
		if (tmp == obj)
		{
			sleep_thread.Delete();
			goto l_recovery;
		}
		if (sleep_thread.MoveNext() != S_OK)
		{
			goto l_recovery;
		}
	}
l_recovery:

	/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
	/* δ��� */
	return S_OK;
}
