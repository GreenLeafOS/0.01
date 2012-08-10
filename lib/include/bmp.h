/*
 * bmp.h
 *
 *  Created on: 2012-8-10
 *      Author: greenleaf
 */

#ifndef BMP_H_
#define BMP_H_

#include <lib/include/type.h>
#include <lib/include/setbit.h>


#define BmpArrayDefine(name,datatype,max,type)	\
		type datatype name[max];					\
		type u32 name##_data[max/32]


static inline void bmp_set(void *bmp,u32 bit)
{
	BITSET((u32*)bmp,bit);
}

static inline void bmp_clear(void *bmp,u32 bit)
{
	BITCLEAR((u32*)bmp,bit);
}

#define Bmp ((u32*)bmp)
static inline int bmp_search(void *bmp,int max)
{
	for(int i=0;i<max;i++)
	{
		u32* bmp_int = *(Bmp + i);
		if (*bmp_int == 0xffffffff) continue;
		for(int j=0;j<32;j++)
		{
			if (!(BITTEST(bmp_int,j)))
			{
				return ((i*32)+j);
			}
		}
	}
	return -1;
}
#endif /* BMP_H_ */
