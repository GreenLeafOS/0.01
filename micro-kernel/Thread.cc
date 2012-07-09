/*===============================================================
 *                      Thread.cc
 *                     	内核线程 实现
 *                     							作者：柳暗花明-绿叶
 ===============================================================*/
#include "Thread.h"

using namespace Thread;
/*===============================================================
 *                        数据区
 ===============================================================*/
u32 			Thread::kernel_esp;
KernelThread* 	Thread::run_thread;
ThreadList 		Thread::ready_thread;
ThreadList 		Thread::sleep_thread;
