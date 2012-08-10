/*
 * Module.h
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "lib/include/stack_array.h"

// ģ��ͷ��Ϣ
typedef struct module_head
{
	FunAddr fun_msg_handle;
	FunAddr fun_init;
	u16 id;
}ModHead;

/* ����ģ�� */
int mod_load(u32 mod_addr);

/* ���ݶ��� */
StackArrayDefine(mod_table,u32,32,extern);

#endif /* MODULE_H_ */
