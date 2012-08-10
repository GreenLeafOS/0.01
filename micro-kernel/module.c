/*
 * module.c
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#include "include/kernel.h"

/* ģ��ע��� */
StackArrayStruct(mod_table,ModuleHead*,32,);

/* ����ģ�� */
int mod_load(ModuleHead* p_mod)
{
	int id = stack_array_add(&mod_table_data,(point)p_mod);
	if (id != -1)
	{
		p_mod->id = id;
		(*(p_mod->fun_init))();
	}
	return id;
}
