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


/* handle 16byte 一个页256项 */
struct handle
{
	u32 				id;		// id（为0表示未使用）
	void*				p;		// 资源指针
	struct handle_info	info;	// 句柄信息
};



/* mod_handle config */
#define HANDLE_TABLE_BLOCK_SIZE		8			// 2的8次方，256页
#define HANDLE_TABLE_BLCOK_MAX		256


/* mod_handle data */
id_t 				mod_handle_id;
struct handle* 		mod_handle_table[HANDLE_TABLE_BLCOK_MAX];



/*
 * mod_handle_do
 * 参数：消息
 * 功能：处理消息
 * 返回值：无
 */
void mod_handle_do(MsgHead msg)
{
	return;
}





/*
 * mod_handle_main
 * 参数：无
 * 功能：主循环
 * 返回值：无
 */
void mod_handle_main()
{
	while(1)
	{
		MsgHead msg = recv();		// 获取消息
		mod_handle_do(msg);			// 处理消息
	}
}

