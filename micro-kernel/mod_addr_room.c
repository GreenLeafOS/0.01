/*
 * mod_addr_room.c
 *
 *  Created on: 2012-8-20
 *      Author: greenleaf
 */


#include "include/sysapi.h"
#include "include/module.h"

/* addr_room  macro */
#define UserLinearSpacePages		524288		// 2G



/* addr_room data */
id_t mod_addr_room_id;



/*
 * mod_addr_room_create
 * 创建线性地址空间
 */
Handle mod_addr_room_create()
{
	Handle ret = new();
	LinearRoom room;

	list_init(&room.head);
	room.free_count = UserLinearSpacePages;
	room.table = (PageTable*)alloc(0);

	// 以ret为句柄，存入room

	return ret;
}




/*
 * mod_addr_room_alloc
 * 分配线性块
 */
Handle mod_addr_room_alloc()
{
	Handle ret = new();
	LinearBlock block;

	// 以ret为句柄，存入block

	return ret;
}




/*
 * mod_addr_room_add
 * 添加内存块对象到地址空间
 */
Bool mod_addr_room_add(Handle addr_room,Handle linear_block)
{
	int i=0,pages=0;

	LinearBlock *other;
	// 获取linear_block的实例
	LinearBlock *block;
	// 获取addr_room的实例
	LinearRoom *room;

	/* 新块的起始和结束 */
	int start = block->start;
	int end = block->start + block->block->private;


	/* 只要获得的指针不为NULL，搜索新块位置是否无效 */
	while(other = (LinearBlock*)list_search(&room.head,i))
	{
		/* 已有块的起始和结束 */
		int other_start = other->start;
		int other_end = other->start + other->block->private;

		if ((other_start >= start && other_start <= end) ||	/* 已有块的起始介于新块区域之间 */
			(other_end >= start   && other_end <= end))		/* 已有块的结束介于新块区域之间 */
		{
			return False;
		}
		i++;
	}

	/* 没有返回，则直接插入双向链表 */
	list_add(&room->head,&block->block->node);

	/* 空闲数减小 */
	OrderToPages(block->block->private,pages);
	room->free_count -= pages;

	return True;
}




/*
 * mod_addr_room_delete
 * 从地址空间中删除对象
 */
void mod_addr_room_del(Handle addr_room,Handle linear_block)
{
	// 获取linear_block的实例
	LinearBlock *block;
	// 获取addr_room的实例
	LinearRoom *room;

	list_unlink(&room->head,&block->block->node);
}





/*
 * mod_addr_room_switch
 * 切换地址空间
 */
void mod_addr_room_switch(Handle addr_room)
{
	// 获取addr_room的实例
	LinearRoom	*room;
	LinearBlock	*block;


	/* 只要获得的指针不为NULL，循环搜索块 */
	while(block = (LinearBlock*)list_search(&room.head,i))
	{
		/* 如果块没有映射过 */
		if (!(bmp_test(&block->flags,LB_USED)))
		{
			for(int i=block->start;i<block->start + block->block->private;i++)
			{
				u32 page_index = (block->start + i) & 0x3FF;	/* 取得页号，高位屏蔽 */
				/* 页目录项下标等于起始页号除以一个项可以映射的页数 */
				PageEntry *pde = (PageEntry*)&room->table->items[page_index/1024];
				PageTable *tbl;
				PageEntry *pte;

				/* 如果页表没有分配 */
				if (pde->avl != PDE_ALLOCATED)
				{
					/* 就马上分配一个页 */
					pde->addr = (u32)alloc(0);
				}
				tbl = (PageTable*)(u32)pde.addr;

				/* 页表项的下标等于起始页号除以一个项可以映射的页数的余数 */
				pte = (PageEntry*)tbl->items[page_index%1024];

				/* 映射 */
				pte->addr = (u32)page_index;
				pte->p = 1;
				pte->avl = PDE_ALLOCATED;
			}
		}
		i++;
	}

	page_directory_load((u32)&room->table);	/* 加载页表 */
}




/*
 * mod_addr_room_do
 * 处理消息
 */
void mod_addr_room_do(MsgHead msg)
{
	switch(msg.vector)
	{
		/* 创建资源 */
		case MSG_REQUEST_CREATE:
		{
			struct _body_create* body = (struct _body_create*)msg.body_point;
			/* 线性地址空间 */
			if (body->type ==  MSG_ADDRROOM_CREATE_ROOM)
			{
				*body->ret = mod_addr_room_create();
				goto Success;
			}
			/* 线性块 */
			else
			{
				*body->ret = mod_addr_room_alloc();
				goto Success;
			}
		}
		break;


		/* 添加 */
		case MSG_ADDRROOM_ADD:
		{
			struct _body_addr_room_add_del* body = (struct _body_addr_room_add_del*)msg.body_point;
			if (mod_addr_room_add(body->room,body->block))
				goto Success;
			else
				goto Fail;

		}
		break;

		/* 删除 */
		case MSG_ADDRROOM_DEL:
		{
			struct _body_addr_room_add_del* body = (struct _body_addr_room_add_del*)msg.body_point;
			mod_addr_room_del(body->room,body->block);
			goto Success;
		}
		break;


		/* 删除 */
		case MSG_ADDRROOM_SWITCH:
		{
			struct _body_addr_room_switch* body = (struct _body_addr_room_switch*)msg.body_point;
			mod_addr_room_switch(body->room);
			goto Success;
		}
		break;
	}
	return;

Success:
	msg_ok.sender = mod_addr_room_id;
	msg_ok.receiver = msg.sender;
	post(msg_ok);
	return;

Fail:
	msg_fail.sender = mod_addr_room_id;
	msg_fail.receiver = msg.sender;
	post(msg_fail);
	return;

}



/*
 * mod_addr_room_main
 * 主循环
 */
void mod_addr_room_main()
{
	char* str = "linear address space.\n";
	print(str);
	while(1)
	{
		MsgHead msg = recv();		/* 获取消息 */
		mod_addr_room_do(msg);		/* 处理消息 */
	}
}



