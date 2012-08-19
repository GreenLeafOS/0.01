/*
 * main.c
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#include "include/main.h"
#include "include/sysapi.h"
#include "include/handle.h"
#include "include/module.h"

/* main data */
u32* kernel_stack_top;
int kernel_reenter;


void main_thread_create();
void public_msg_main();

void kernel_main()
{
	char *str = "GreenLeafOS version 0.01.\n";
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
 * 主线程
 */
void kernel_main_thread()
{
	char* str = "main thread.\n";
	print(str);
	while(1);
}


void main_thread_create()
{
	KernelThread* thread;

	/* 建立线程 */
	thread_default_reg.eip = (u32)public_msg_main;
	CreateThread(thread,thread_default_reg);


	thread_default_reg.eip = (u32)mod_time_main;
	CreateThread(thread,thread_default_reg);
	mod_time_id = thread->thread_info.id;
}
