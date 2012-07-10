#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	�ں��߳� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "DataStruct.h"
#include "Lock.h"

/*===========�߳������ļĴ����ṹ=============*/
typedef struct stack_frame {
	/* ����ѹ�� */
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	/* pushalѹ��*/
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	/* ϵͳѹ�� */
	u32	eip;
	u32	cs;
	u32	eflags;
	u32	esp;
	u32	ss;
}StackFrame;
#define REGFRAME_HIGH 	&Thread::run_thread->thread_info.reg.eax
#define REGFRAME_LOW 	&Thread::run_thread->thread_info.reg.gs

/*==============�߳�������================*/
struct kernel_thread_desc
{
	u16 id;			/* �߳�ID */
	u16 state;		/* ״̬��Ϣ */
	u32 rec_des;	/* ռ����Դ������ */
	FunAddr save_fun;		/* ר�еı���״̬�ĺ��� */
	FunAddr restart_fun;	/* ר�еĻָ�״̬�ĺ��� */
	StackFrame reg;	/* �Ĵ���ֵ */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4


/*==============�߳�ջ==================*/
typedef union thread_union
{
	struct kernel_thread_desc thread_info;
	u32 stack[1024];		/* 4k��ջ */
}KernelThread;
#define NR_THREAD 32



/*==============Thread�����ռ�==================*/
namespace Thread
{
/*************************�߳�״̬���ƺ���**********************************/
	/* �����߳�
	 * ����һ���߳�������
	 * ��ʼ���߳�������
	 * ���ھ����߳����������һ����
	 * ��ʼ��ջ
	 * ������ַ�Ͳ���ѹ���̵߳�ջ */
	Result Create(ThreadFun fun,point params,u16 &ret_id);

	/* ˯���߳�
	 * �����������̬,����������,��ͨ������Save()������
	 * ���̵߳�״̬��Ϊ˯��̬
	 * �Ӿ��������ж������Ƴ�
	 * ����˯�߶��� */
	Result Sleep(u16 id);

	/* �����߳�
	 * ���̵߳�״̬��Ϊ����̬
	 * ��˯�߶������Ƴ�
	 * ����������� */
	Result Wake(u16 id);

	/* ɱ���߳�
	 * ���̴߳��κ�һ���������Ƴ�
	 * �ͷ��߳�ռ�е��κ���Դ(���͹㲥��Ϣ) */
	Result Kill(u16 id);

	/* �����̵߳��Զ���ָ������溯��
	 * ���ֻ�������֧���漰���ȼ�ת�����߳� */
	inline Result SetFun(int id,FunAddr save,FunAddr restart);

/***************************�������run_thread***************************/
/****************ȱʡ��������û���ƶ�save_fun,��restart_fun��ʱ��**********/

	/* ����run_thread��ͨ�üĴ����Ͳ��ֶμĴ������߳�ջ */
	void Save();
	/* �ָ��������߳���ִ�� */
	void Restart();


	/* �����㷨,ѡ��һ���߳���Ϊrun_thread */
	void Schedule();

/*******************************������***********************************/
	typedef ShareLinkList<KernelThread*,NR_THREAD> ThreadList;
	typedef BmpArray<struct link_item<KernelThread*>,NR_THREAD> StaticData;
	extern u32 kernel_esp;
	extern KernelThread* run_thread;
	extern ThreadList ready_thread;
	extern ThreadList sleep_thread;
};

/************************************************************************/
/*					�����̵߳��Զ���ָ������溯��
/*                          SetFun
/************************************************************************/
inline Result Thread::SetFun(int id,FunAddr save,FunAddr restart)
{
	KernelThread*  *p_thread;

	/* ��ȡ�߳�ָ�� */
	if (ready_thread.GetAddr(id,&p_thread) != S_OK) return E_NOITEM;

	/* ���ú��� */
	LOCK()
		p_thread->thread_info.save_fun = save;
		p_thread->thread_info.restart_fun = restart;
	UNLOCK()

	return S_OK;
}


#endif /* KERNEL_H_ */
