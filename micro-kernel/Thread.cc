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
