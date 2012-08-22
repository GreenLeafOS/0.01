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
 * �������Ե�ַ�ռ�
 */
Handle mod_addr_room_create()
{
	Handle ret = new();
	LinearRoom room;

	list_init(&room.head);
	room.free_count = UserLinearSpacePages;
	room.table = (PageTable*)alloc(0);

	// ��retΪ���������room

	return ret;
}




/*
 * mod_addr_room_alloc
 * �������Կ�
 */
Handle mod_addr_room_alloc()
{
	Handle ret = new();
	LinearBlock block;

	// ��retΪ���������block

	return ret;
}




/*
 * mod_addr_room_add
 * ����ڴ����󵽵�ַ�ռ�
 */
Bool mod_addr_room_add(Handle addr_room,Handle linear_block)
{
	int i=0,pages=0;

	LinearBlock *other;
	// ��ȡlinear_block��ʵ��
	LinearBlock *block;
	// ��ȡaddr_room��ʵ��
	LinearRoom *room;

	/* �¿����ʼ�ͽ��� */
	int start = block->start;
	int end = block->start + block->block->private;


	/* ֻҪ��õ�ָ�벻ΪNULL�������¿�λ���Ƿ���Ч */
	while(other = (LinearBlock*)list_search(&room.head,i))
	{
		/* ���п����ʼ�ͽ��� */
		int other_start = other->start;
		int other_end = other->start + other->block->private;

		if ((other_start >= start && other_start <= end) ||	/* ���п����ʼ�����¿�����֮�� */
			(other_end >= start   && other_end <= end))		/* ���п�Ľ��������¿�����֮�� */
		{
			return False;
		}
		i++;
	}

	/* û�з��أ���ֱ�Ӳ���˫������ */
	list_add(&room->head,&block->block->node);

	/* ��������С */
	OrderToPages(block->block->private,pages);
	room->free_count -= pages;

	return True;
}




/*
 * mod_addr_room_delete
 * �ӵ�ַ�ռ���ɾ������
 */
void mod_addr_room_del(Handle addr_room,Handle linear_block)
{
	// ��ȡlinear_block��ʵ��
	LinearBlock *block;
	// ��ȡaddr_room��ʵ��
	LinearRoom *room;

	list_unlink(&room->head,&block->block->node);
}





/*
 * mod_addr_room_switch
 * �л���ַ�ռ�
 */
void mod_addr_room_switch(Handle addr_room)
{
	// ��ȡaddr_room��ʵ��
	LinearRoom	*room;
	LinearBlock	*block;


	/* ֻҪ��õ�ָ�벻ΪNULL��ѭ�������� */
	while(block = (LinearBlock*)list_search(&room.head,i))
	{
		/* �����û��ӳ��� */
		if (!(bmp_test(&block->flags,LB_USED)))
		{
			for(int i=block->start;i<block->start + block->block->private;i++)
			{
				u32 page_index = (block->start + i) & 0x3FF;	/* ȡ��ҳ�ţ���λ���� */
				/* ҳĿ¼���±������ʼҳ�ų���һ�������ӳ���ҳ�� */
				PageEntry *pde = (PageEntry*)&room->table->items[page_index/1024];
				PageTable *tbl;
				PageEntry *pte;

				/* ���ҳ��û�з��� */
				if (pde->avl != PDE_ALLOCATED)
				{
					/* �����Ϸ���һ��ҳ */
					pde->addr = (u32)alloc(0);
				}
				tbl = (PageTable*)(u32)pde.addr;

				/* ҳ������±������ʼҳ�ų���һ�������ӳ���ҳ�������� */
				pte = (PageEntry*)tbl->items[page_index%1024];

				/* ӳ�� */
				pte->addr = (u32)page_index;
				pte->p = 1;
				pte->avl = PDE_ALLOCATED;
			}
		}
		i++;
	}

	page_directory_load((u32)&room->table);	/* ����ҳ�� */
}




/*
 * mod_addr_room_do
 * ������Ϣ
 */
void mod_addr_room_do(MsgHead msg)
{
	switch(msg.vector)
	{
		/* ������Դ */
		case MSG_REQUEST_CREATE:
		{
			struct _body_create* body = (struct _body_create*)msg.body_point;
			/* ���Ե�ַ�ռ� */
			if (body->type ==  MSG_ADDRROOM_CREATE_ROOM)
			{
				*body->ret = mod_addr_room_create();
				goto Success;
			}
			/* ���Կ� */
			else
			{
				*body->ret = mod_addr_room_alloc();
				goto Success;
			}
		}
		break;


		/* ��� */
		case MSG_ADDRROOM_ADD:
		{
			struct _body_addr_room_add_del* body = (struct _body_addr_room_add_del*)msg.body_point;
			if (mod_addr_room_add(body->room,body->block))
				goto Success;
			else
				goto Fail;

		}
		break;

		/* ɾ�� */
		case MSG_ADDRROOM_DEL:
		{
			struct _body_addr_room_add_del* body = (struct _body_addr_room_add_del*)msg.body_point;
			mod_addr_room_del(body->room,body->block);
			goto Success;
		}
		break;


		/* ɾ�� */
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
 * ��ѭ��
 */
void mod_addr_room_main()
{
	char* str = "linear address space.\n";
	print(str);
	while(1)
	{
		MsgHead msg = recv();		/* ��ȡ��Ϣ */
		mod_addr_room_do(msg);		/* ������Ϣ */
	}
}



