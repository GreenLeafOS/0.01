/*
 * mod_addr_room.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */


#include "include/sysapi.h"
#include "include/module.h"


typedef struct linear_block_object
{
	u32 	flags;			// ���Կռ��״̬������
	Handle	objs[4];		// һ�����Կ����4����Դ���,��������ҳ����������
							// Ҳ�������������豸����������ʹ���ļ��������ڴ����ں˿�������һ����
}MemObj;




typedef struct linear_room
{
	ListHead mem_obj;		// ���Կռ���˫������ͷָ��
	u32 free_count;			// ʣ��Ŀռ仹�ж���ҳ��
};
