/*
 * memory.c
 *
 *  Created on: 2012-8-6
 *      Author: greenleaf
 */

#include "memory.h"

/*
 * ��ʼ��
 * ����ڴ��С
 * ����λͼ��С
 * λͼ��һ��λ��ʾһ��ҳ��ʹ�����
 * ��һ����ɱ�ʾ32��ҳ��32*4096/1024 = 128 Kb = 1/8 M
 * 8��������� 128*8/1024 = 1 M
 */

void mem_init()
{
	/* ����ڴ��С */
	for(int i=0;i<mem_mcr_number;i++)
	{
		if(mem_info[i].Type == 1)
			if (mem_info[i].BaseAddrLow + mem_info[i].LenthLow > mem_size)
				mem_size = mem_info[i].BaseAddrLow + mem_info[i].LenthLow;
	}

	/* ����λͼ��С */
	mem_used_map_max = (mem_size/PAGE_SIZE)/32 + 1;
	mem_size = mem_used_map_max*32*PAGE_SIZE;

	/* ��ָ�븳ֵ */
	mem_used_map =(u32*)0x16000;

	/* ����4M���ں˿ռ� */
	for(int i=0;i<MEM_BMP_MB(4);i++)
		*(mem_used_map + i) = ~0;

	/* ��������Ϊ0 */
	for(int i=MEM_BMP_MB(4);i<mem_used_map_max;i++)
		*(mem_used_map + i) = 0;
}



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
				return (index*PAGE_SIZE);
			}
		}
	}
	return 0xffffffff;
}
