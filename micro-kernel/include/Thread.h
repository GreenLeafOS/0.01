#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	�ں��߳� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "DataStruct.h"


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
	/* ĳĳѹ�� */
	u32	retaddr;
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
struct kernel_thread
{
	u16 id;			/* �߳�ID */
	u16 state;		/* ״̬��Ϣ */
	u32 rec_des;	/* ռ����Դ������ */
	StackFrame reg;	/* �Ĵ���ֵ */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4


/*==============�߳�ջ==================*/
typedef union thread_union
{
	struct kernel_thread thread_info;
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
	 * ������ַ�Ͳ���ѹ���̵߳�ջ
	 */
	Result Create(ThreadFun fun,point params);

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


/***************************�������run_thread***************************/

	/* ����run_thread��ͨ�üĴ����Ͳ��ֶμĴ������߳�ջ */
	inline void Save();

	/* �ָ�run_thread�������ģ����ѿ���Ȩ�����߳� */
	inline void Restart();

	/* �����㷨,ѡ��һ���߳���Ϊrun_thread */
	void Schedule();

/*******************************������***********************************/
	typedef ShareLinkList<KernelThread*,NR_THREAD> ThreadList;

	extern u32 kernel_esp;
	extern KernelThread* run_thread;
	extern ThreadList ready_thread;
	extern ThreadList sleep_thread;
};

/************************************************************************/
/*                      ����Ĵ�������
/*                         Save
/************************************************************************/
inline void Thread::Save()
{
	/* ����ԭesp��ֵ��kernel_esp������ */
	asm volatile("movl %%esp,%0"
				:"+m"(Thread::kernel_esp)
				);

	/* espָ�� �߳�ջ�б���ͨ�üĴ������ֵ���ߵ�ַ */
	asm volatile("movl %0,%%esp"
				:
				:"m"(RUNTHREAD_REG_HIGH)
				);

	/*����ͨ�üĴ�����һЩ�μĴ���*/
	asm volatile("cld 		\n"
				"pushal		\n"
				"push %ds	\n"
				"push %es	\n"
				"push %fs	\n"
				"push %gs	\n"
				);

	/* �ָ�esp */
	asm volatile("movl %0,%%esp"
				:
				: "m"(Thread::kernel_esp)
				);
	return;
}
/************************************************************************/
/*                      �ָ��Ĵ�������
/*                        Restart
/************************************************************************/
inline void Thread::Restart()
{
	/* ����ԭesp��ֵ��kernel_esp������ */
	asm volatile("movl %%esp,%0"
				:"+m"(Thread::kernel_esp)
				);

	/* espָ�� �߳�ջ�б���ͨ�üĴ������ֵ���͵�ַ */
	asm volatile("movl %0,%%esp"
				:
				:"m"(REGFRAME_LOW)
				);

	/*�ָ�ͨ�üĴ�����һЩ�μĴ���*/
	asm volatile("cld 			\n"
				"pop %gs		\n"
				"pop %fs		\n"
				"pop %es		\n"
				"pop %ds		\n"
				"popal			\n"
				"addl $4,%esp	\n"
				"iretd"
				);
}
#endif /* KERNEL_H_ */
