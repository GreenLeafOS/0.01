/*
 * mod_handle.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */


#include "include/module.h"
#include "include/sysapi.h"


/* mod_handle data */
id_t mod_time_id;


struct handle
{

};


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

