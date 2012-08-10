/*
 * module.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#include "include/module.h"

StackArrayStruct(mod_table,u32,32,);

/* ¼ÓÔØÄ£¿é */
int mod_load(u32 mod_addr)
{
	int id = stack_array_add(&mod_table_data,(point)mod_addr);
	if (id != -1)
	{
		((ModHead*)mod_addr)->id = id;
		FunAddr init = ((ModHead*)mod_addr)->fun_init;
		(*init)();
	}
	return id;
}
