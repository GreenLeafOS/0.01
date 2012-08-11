/*
 * list.c
 *
 *  Created on: 2012-8-11
 *      Author: greenleaf
 */

#include "include/list.h"


/*
 * ˫�����ʼ��
 * */
void list_init(ListHead *head)
{
	head->prev = NULL;
	head->next = NULL;
	return;
}



/*
 * ����ڿ�ͷ
 * */
Bool list_add(ListHead *head,ListNode *node)
{
	if(head == NULL)
	{
		return False;
	}
	if(head->next == NULL)
	{
		node->prev = (ListNode*)head;
		node->next = NULL;
		head->next = node;
		return True;
	}
	node->prev =(ListNode*)head;	/* ����ָ���ǰָ��Ϊͷָ�� */
	node->next = head->next;		/* ����ָ��ĺ�ָ��Ϊͷָ��ĺ�ָ�� */
	head->next->prev = node;		/* ͷָ��ĺ�ָ���ǰָ��Ϊ����ָ�� */
	head->next = node;				/* ͷָ��ĺ�ָ���Ϊ����ָ�� */
	return True;
}




/*
 * ����
 */
ListNode* list_search(ListHead *head,int index)
{
	ListNode *node;
	for(node=head->next;index > 0;index--)
	{
		if(node == NULL) return NULL;
		node = node->next;
	}
	return node;
}




/*
 * ����
 * */
Bool list_insert(ListHead *head,ListNode *node,int index)
{
	ListNode *old_node = list_search(head,index);
	if (old_node == NULL) return False;
    node->next = old_node->next;
    node->prev = old_node;
	if (old_node->next != NULL)
	{
		old_node->next->prev = node;
    }
	old_node->next = node;
    return True;
}



/*
 * �������
 * */
void list_unlink(ListNode *node)
{
	if (node->prev != NULL)
	{
		node->prev->next = node->next;	/* ��һ��Ԫ�ص�ǰָ��ָ��ǰһ��Ԫ�� */
		return;
	}
	if (node->next != NULL)
	{
		node->next->prev = node->prev;	/* ǰһ��Ԫ�صĺ�ָ��ָ���һ��Ԫ�� */
		return;
	}
    return ;
}


/*
 * ɾ��
 * */
Bool list_delete(ListHead *head,int index)
{
	ListNode *old_node = list_search(head,index);
	if (old_node == NULL) return False;
	list_unlink(old_node);
	return True;
}



/*
 * �ж��Ƿ�Ϊ��
 * */
Bool list_empty(ListHead *head)
{
    return head->next == NULL?True:False;
}
