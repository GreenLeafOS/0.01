/*
 * memory.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "include/memory.h"


/*
 * ����ĳ��ҳΪʹ��
 * ��λͼ�У�1��ʾʹ�ã�0��ʾ����
 * */
void mem_page_busy(u32 index)
{
	if ((index * PAGE_SIZE) > mem_size) return;
	BITSET(mem_used_map,index);
}



/*
 * ����ĳ��ҳΪ����
 * ��λͼ�У�1��ʾʹ�ã�0��ʾ����
 * */
void mem_page_free(u32 index)
{
	if ((index * PAGE_SIZE) > mem_size) return;
	BITCLEAR(mem_used_map,index);
}



/*
 * ����һ������ҳ
 * ��λͼ�У�1��ʾʹ�ã�0��ʾ����
 * ����ҳ���
 * */
u32 mem_page_getfree()
{
	for(int i=0;i<mem_used_map_max;i++)
	{
		u32 l_bmp = *(mem_used_map + i);
		if (l_bmp == 0xffffffff) continue;
		for(int j=0;j<32;j++)
		{
			if (!(BITTEST(&l_bmp,j)))
			{
				return ((i*32)+j);
			}/* end if */
		}/* end for */
	}/* end for*/
	return -1;
}


/*
 * ����һ��ҳ
 * ���ص�ַ
 * ebp - 20 j
 */
void* mem_page_alloc()
{
	for(int i=0;i<mem_used_map_max;i++)
	{
		u32 l_bmp = *(mem_used_map + i);
		if (l_bmp == 0xffffffff) continue;
		for(int j=0;j<32;j++)
		{
			if (!(BITTEST(&l_bmp,j)))
			{
				int index = (i*32)+j;
				BITSET(mem_used_map,index);
				return (void*)(index*PAGE_SIZE);
			}
		}
	}
	return (void*)0xffffffff;
}
