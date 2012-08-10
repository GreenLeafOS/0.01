#ifndef TYPE_H
#define TYPE_H

#define NULL	'\0'

typedef unsigned int size_t;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef void* point;
typedef void (*MsgHandle)(point params);
typedef void (*ThreadFun)(point params);
typedef void (*FunAddr)();
struct point_list_info
{
	point *head;
	point *tail;
};

struct int_list_info
{
	u8	head;
	u8	tail;
};


typedef unsigned long Result;

#define S_OK			1
#define E_MAX			-1
#define E_MSG			-2
#define E_NOFREE		-3
#define E_NOITEM		-4
#define E_USED			-5
#define E_NOFUN			-6
#define E_LOCK			-7
#define E_EMPTY			-8
#define E_FULL			-9

#endif
