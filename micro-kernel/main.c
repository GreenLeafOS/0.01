/*
 * main.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/main.h"
#include "include/sysapi.h"
#include "include/irq.h"
#include "include/module.h"

/* main data */
u32* kernel_stack_top;
int kernel_reenter;

/*
 * 内核主函数
 */
void kernel_main()
{
	char *str = "\n\nGreenLeafOS version 0.01.\n";
	print(str);

	kernel_stack_top = (u32*)&tss.esp0;
	kernel_reenter = 0;
	thread_init();

	main_thread_create();

	thread_schedule();
	restart();

	while(1);
}





/*
 * kernel_main_thread
 * 主线程
 */
void kernel_main_thread()
{
	while(1)
	{
		char* in = "m";
		for(int i=0;i<1000000;i++);
		print(in);
	}
	while(1);
}



/*
 * test
 */
int test_id;
void test_main()
{
	char *node = "test.\n";
	print(node);

	Handle handle;
	struct _body_create body =
	{
			0,
			&handle
	};
	MsgHead msg =
	{
			MSG_REQUEST_CREATE,
			MSG_PRIORITY_KERNEL,
			test_id,
			mod_addr_room_id,
			&body,
			sizeof(body),
			0
	};
	post(msg);
	recv();

	char str[16];
	char *info = "new linear address space.  handle:";
	itoa(handle,(char*)&str);

	print(info);
	print(str);

	while(1)
	{
		char* in = "o";
		for(int i=0;i<10000000;i++);
		print(in);
	}
}


/*
 * main_thread_create
 * 创建线程
 */
void main_thread_create()
{
	KernelThread* thread;

	/* 建立线程 */
	CreateThread(mod_public_msg,	thread,		thread_default_reg);
	CreateThread(mod_time,			thread,		thread_default_reg);
	CreateThread(mod_addr_room,		thread,		thread_default_reg);
	CreateThread(test,				thread,		thread_default_reg);
}
