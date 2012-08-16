/*
 * message.h
 *
 *  Created on: 2012-8-16
 *      Author: greenleaf
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include "lib.h"



/* MsgHead */
typedef struct message_head
{
	u16		vector;					/* ��Ϣ���� */
	u16		priority;				/* ��Ϣ���ȼ� */
	id_t	sender;					/* ������ */
	id_t	receiver;				/* ������ */
	point	body_point;				/* ��Ϣ��ָ�� */
	u16		body_size;				/* ��Ϣ���С */
}MsgHead;


/* MsgHead.priority */
#define MSG_PRIORITY_REALTIME	0
#define MSG_PRIORITY_KERNEL		1
#define MSG_PRIORITY_DRIVER		2
#define MSG_PRIORITY_USER		3

/* MsgHead.vector */
#include "vector.h"


#endif /* MESSAGE_H_ */
