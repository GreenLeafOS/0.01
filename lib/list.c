/*
 * list.c
 *
 *  Created on: 2012-8-11
 *      Author: greenleaf
 */

#include "include/list.h"


/*
 * 双链表初始化
 * */
void list_init(ListHead *head)
{
	head->prev = NULL;
	head->next = NULL;
	return;
}



/*
 * 添加在开头
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
	node->prev =(ListNode*)head;	/* 加入指针的前指针为头指针 */
	node->next = head->next;		/* 加入指针的后指针为头指针的后指针 */
	head->next->prev = node;		/* 头指针的后指针的前指针为加入指针 */
	head->next = node;				/* 头指针的后指针变为加入指针 */
	return True;
}




/*
 * 搜索
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
 * 插入
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
 * 解除链接
 * */
void list_unlink(ListNode *node)
{
	if (node->prev != NULL)
	{
		node->prev->next = node->next;	/* 后一个元素的前指针指向前一个元素 */
		return;
	}
	if (node->next != NULL)
	{
		node->next->prev = node->prev;	/* 前一个元素的后指针指向后一个元素 */
		return;
	}
    return ;
}


/*
 * 删除
 * */
Bool list_delete(ListHead *head,int index)
{
	ListNode *old_node = list_search(head,index);
	if (old_node == NULL) return False;
	list_unlink(old_node);
	return True;
}



/*
 * 判断是否为空
 * */
Bool list_empty(ListHead *head)
{
    return head->next == NULL?True:False;
}
