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

