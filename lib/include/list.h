/*===============================================================
 *                        list.h
 *                	      ˫������
 ===============================================================*/
#ifndef _LIST_H
#define _LIST_H

#include <lib/include/type.h>

/* �ڵ�û�����ݣ��������������� */
typedef struct list_node
{
    /*char i; */
    struct list_node* next;
    struct list_node* prev;
}ListNode,*pListNode;

/* ����ͷ�ṹ */
typedef struct list_head
{
	struct list_node* next;
}ListHead,*pListHead;

/* ������� */
pListNode list_create(char );
/* ˫�����ʼ�� */
pListHead list_init();
/* ����ڿ�ͷ */
Bool list_add(pListHead head,pListNode node);
/* ���� */
Bool list_insert(pListHead head,pListNode node,int index);
/* ɾ�� */
void list_delete(pListHead head);
/* �ж��Ƿ�Ϊ�� */
Bool list_empty(pListHead head);
/* ˫�������� */
void list_destroy(pListHead head);
/* �������� */
void list_enum(pListHead head);

#endif
