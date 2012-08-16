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
	u16		vector;					/* 消息向量 */
	u16		priority;				/* 消息优先级 */
	id_t	sender;					/* 发送者 */
	id_t	receiver;				/* 接收者 */
	point	body_point;				/* 消息体指针 */
	u16		body_size;				/* 消息体大小 */
}MsgHead;


/* MsgHead.priority */
#define MSG_PRIORITY_REALTIME	0
#define MSG_PRIORITY_KERNEL		1
#define MSG_PRIORITY_DRIVER		2
#define MSG_PRIORITY_USER		3

/* MsgHead.vector */
#include "vector.h"


#endif /* MESSAGE_H_ */
