/*
 * handle.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */

#include "include/sysapi.h"
#include "include/handle.h"

/* handle data */
Handle*	handle_table[HANDLE_TABLE_BLCOK_MAX];




/*
 * SysApi new
 * �����������Ϣ��������дid��
 * ���ܣ�����һ��������
 * ����ֵ�����ָ��
 */
Handle*	new(Handle handle)
{
	/* ���ѭ�������ұ� */
	for(int i=0;i<HANDLE_TABLE_BLCOK_MAX;i++)
	{
		Handle *free = handle_table[i] ;
		if (free == NULL)
		{
			/* Ϊ�ձ�����ڴ� */
			handle_table[i] = (Handle*)alloc(HANDLE_TABLE_BLOCK_SIZE);
		}
		/* �ڲ�ѭ�������ҿ��о�� */
		for(int j=0;j< HANDLE_TABLE_ITEM_COUNT;j++)
		{
			if (free++->id == 0)
			{
				handle.id = (i*HANDLE_TABLE_ITEM_COUNT)+j;
				*(handle_table[i] + j) = handle;
				return (handle_table[i] + j);
			}
		}
	}
	return NULL;
}





/*
 * SysApi delete
 * ���������ָ��
 * ���ܣ��ͷž��
 * ����ֵ����
 */
void delete(Handle *handle)
{
	(handle_table[0] + handle->id)->id = NULL;
	return;
}





/*
 * SysApi set
 * ��������
 * ���ܣ�д����
 * ����ֵ����
 */
void set(Handle handle)
{
	*(handle_table[0] + handle.id) = handle;
	return;
}




/*
 * SysApi get
 * ��������
 * ���ܣ���ȡ���
 * ����ֵ�����ָ��
 */
Handle get(u32 id)
{
	return *(handle_table[0] + id);
}
