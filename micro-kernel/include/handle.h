/*
 * handle.h
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */

#ifndef HANDLE_H_
#define HANDLE_H_

#include "lib.h"

/* handle_info */
struct handle_info
{
	u32 room[2];
};


/* handle */
typedef struct handle
{
	u32 				id;		// id��Ϊ0��ʾδʹ�ã�
	void*				p;		// ��Դָ��
	struct handle_info	info;	// �����Ϣ
}Handle;


/* handle config */
#define HANDLE_TABLE_BLOCK_SIZE		8			// 2��8�η���256ҳ,һ��ҳ256��
#define HANDLE_TABLE_BLCOK_MAX		256

/* handle macro */
#define HANDLE_TABLE_ITEM_COUNT		((HANDLE_TABLE_BLCOK_MAX*PAGE_SIZE)/sizeof(Handle))


#endif /* HANDLE_H_ */
