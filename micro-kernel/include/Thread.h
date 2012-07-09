#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	�ں��߳� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "DataStruct.h"


/*==============�߳�������================*/
struct kernel_thread
{
	u16 id;			/* �߳�ID */
	u16 state;		/* ״̬��Ϣ */
	u32 rec_des;	/* ռ����Դ������ */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4

/*==============�߳�ջ==================*/
typedef union thread_union
{
	struct kernel_thread thread_info;
	u32 stack[2048];		/* 8k��ջ */
}Thread;
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

	/* ����run_thread�������� */
	void Save();

	/* �ָ�run_thread�������ģ����ѿ���Ȩ�����߳� */
	void Restart();

	/* �����㷨,ѡ��һ���߳���Ϊrun_thread */
	void Schedule();

/*******************************������***********************************/
	typedef ShareLinkList<Thread*,NR_THREAD> ThreadList;

	Thread* run_thread;
	ThreadList ready_thread;
	ThreadList sleep_thread;
}


#endif /* KERNEL_H_ */
