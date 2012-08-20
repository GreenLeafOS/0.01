/*
 * mod_handle.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */


#include "include/module.h"
#include "include/sysapi.h"

/* handle_info */
struct handle_info
{
	u32 room[2];
};


/* handle 16byte һ��ҳ256�� */
struct handle
{
	u32 				id;		// id��Ϊ0��ʾδʹ�ã�
	void*				p;		// ��Դָ��
	struct handle_info	info;	// �����Ϣ
};



/* mod_handle config */
#define HANDLE_TABLE_BLOCK_SIZE		8			// 2��8�η���256ҳ
#define HANDLE_TABLE_BLCOK_MAX		256


/* mod_handle data */
id_t 				mod_handle_id;
struct handle* 		mod_handle_table[HANDLE_TABLE_BLCOK_MAX];



/*
 * mod_handle_do
 * ��������Ϣ
 * ���ܣ�������Ϣ
 * ����ֵ����
 */
void mod_handle_do(MsgHead msg)
{
	return;
}





/*
 * mod_handle_main
 * ��������
 * ���ܣ���ѭ��
 * ����ֵ����
 */
void mod_handle_main()
{
	while(1)
	{
		MsgHead msg = recv();		// ��ȡ��Ϣ
		mod_handle_do(msg);			// ������Ϣ
	}
}

