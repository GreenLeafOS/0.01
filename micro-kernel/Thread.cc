/*===============================================================
 *                      Thread.cc
 *                     	�ں��߳� ʵ��
 *                     							���ߣ���������-��Ҷ
 ===============================================================*/
#include "Thread.h"

using namespace Thread;
/************************************************************************/
/*							������
/************************************************************************/
BmpArray<KernelThread*,NR_THREAD> 	Thread::thread_addr_table;
KernelThread* 						Thread::run_thread;
DoubleLink<NR_THREAD>				Thread::ready_thread;
DoubleLink<NR_THREAD> 				Thread::sleep_thread;

/************************************************************************/
/*							�����߳�
/*                          Create
/************************************************************************/
Result Thread::Create(ThreadFun fun,point params,u16 &ret_id)
{
	/* ����һ���߳������� */
	struct kernel_thread_desc new_thread;

	/* ���ڴ����������һ��ҳ�Ŀռ� */
	KernelThread *new_space;
	/* δ��� */

	/* ���̵߳�ַ�����һ��ָ�� */
	int id = 0;
	int result = thread_addr_table.Add(id,new_space);
	if (result != S_OK) return result;

	/* ��ʼ���߳������� */
	new_thread.id = id;
	new_thread.state = THREAD_STATE_READY;
	new_thread.rec_des = 0;
	new_thread.save_fun = Thread::Save;
	new_thread.restart_fun = Thread::Restart;

	/* ���ھ����߳����������һ���� */
	ready_thread.AddTail(id);
	ret_id = id;

	/* �߳�������д�����߳̿ռ��� */
	new_space->thread_info = new_thread;

	/* ������ַ�Ͳ���ѹ���̵߳�ջ */
	new_space->thread_info.reg.eip = fun;

	/* ��esp��ֵ��Ϊջ�����Ѳ���ѹջ */
	asm volatile("pushl %%edx			\n"
				"movl %%esp,%%edx		\n"		/* �����ں�ջesp */
				"movl %1,%%esp			\n"		/* espָ���߳�ջ�� */
				"pushl %2				\n"		/* ������ջ */
				"movl %%esp,%0			\n"		/* esp���浽reg */
				"movl %%edx,%%esp		\n"		/* �ָ��ں�ջesp */
				"popl %%edx"
				:"+m"(new_space->thread_info.reg.esp)		/* %0,�߳�ջesp�����ַ */
				:"r"((&new_space)+1)						/* %1,�߳�ջ�� */
				 "r"(params)								/* %2,���� */
				);

	return S_OK;
}

/************************************************************************/
/*							˯���߳�
/*                           Sleep
/************************************************************************/
Result Thread::Sleep(u16 id)
{
	KernelThread* thread;
	if(thread_addr_table.Get(id,thread)) return E_NOITEM;

	/* �����������̬,����������,��ͨ������Save()������ */
	if (run_thread->thread_info.id == id)
	{
		CALL_SAVE(run_thread->thread_info)
		run_thread = '\0';
		Schedule();
	}

	/* ���̵߳�״̬��Ϊ˯��̬ */
	thread->thread_info.state = THREAD_STATE_SLEEP;

	/* �Ӿ������Ƴ� */
	if(ready_thread.Search(id))
		ready_thread.DeleteNext(id);

	/* ����˯�߶��� */
	sleep_thread.AddTail(id);

	return S_OK;
}
/************************************************************************/
/*							�����߳�
/*                           Wake
/************************************************************************/
Result Thread::Wake(u16 id)
{
	KernelThread* thread;
	if(thread_addr_table.Get(id,thread)) return E_NOITEM;

	/* ���̵߳�״̬��Ϊ����̬ */
	thread->thread_info.state = THREAD_STATE_READY;

	/* ��˯�߶������Ƴ� */
	sleep_thread.DeleteNext(id);

	/* ����������� */
	ready_thread.AddTail(id);

	return S_OK;
}
/************************************************************************/
/*							ɱ���߳�
/*                           Kill
/************************************************************************/
Result Thread::Kill(u16 id)
{
	/* ���̴߳��̵߳�ַ�����Ƴ� */
	thread_addr_table.Delete(id);

	/* ���̴߳��κ�һ���������Ƴ� */
	if(run_thread == obj)
	{
		run_thread = '\0';
	}
	else if(ready_thread.Search(id))
	{
		ready_thread.DeleteNext(id);
	}
	else if(sleep_thread.Search(id))
	{
		sleep_thread.DeleteNext(id);
	}

	/* ���͹㲥��Ϣ��֪ͨ��������������Դ*/
	// SendMessage(MOD_FREE_SOURCE,&id);
	/* δ��� */

	return S_OK;
}
