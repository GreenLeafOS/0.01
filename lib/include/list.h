/*===============================================================
 *                        list.h
 *                	      双向链表
 ===============================================================*/
#ifndef _LIST_H
#define _LIST_H

#include <lib/include/type.h>

/* 节点没有数据，仅仅起链接作用 */
typedef struct list_node
{
    /*char i; */
    struct list_node* next;
    struct list_node* prev;
}ListNode,*pListNode;

/* 链表头结构 */
typedef struct list_head
{
	struct list_node* next;
}ListHead,*pListHead;

/* 创建结点 */
pListNode list_create(char );
/* 双链表初始化 */
pListHead list_init();
/* 添加在开头 */
Bool list_add(pListHead head,pListNode node);
/* 插入 */
Bool list_insert(pListHead head,pListNode node,int index);
/* 删除 */
void list_delete(pListHead head);
/* 判断是否为空 */
Bool list_empty(pListHead head);
/* 双链表销毁 */
void list_destroy(pListHead head);
/* 遍历链表 */
void list_enum(pListHead head);

#endif
