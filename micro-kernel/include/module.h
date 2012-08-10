/*
 * Module.h
 *
 *  Created on: 2012-8-8
 *      Author: greenleaf
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "lib/include/stack_array.h"

// 模块头信息
typedef struct module_head
{
	FunAddr fun_msg_handle;
	FunAddr fun_init;
	u16 id;
}ModHead;

/* 加载模块 */
int mod_load(u32 mod_addr);

/* 数据定义 */
StackArrayDefine(mod_table,u32,32,extern);

#endif /* MODULE_H_ */
