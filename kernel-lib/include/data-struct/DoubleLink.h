#ifndef DOUBLELINK_H
#define DOUBLELINK_H
/*===============================================================
 *                      DoubleLink.h
 *                    数据结构-双向链表
 *													绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"
#include "BmpArray.h"


struct link_node
{
	int prev;
	int next;
};

template<unsigned int Max>
class DoubleLink  
{
public:
	void Init();
	
	// 插入第一个元素
	void SetFirst(int id);

	// 正序查找
	int ListTraverse(int pos);
	// 逆序查找
	int ListTraverseBack(int pos);
	// 添加
	bool InsertPrev(int id,int index);
	bool InsertNext(int id,int index);
	// 删除
	int DeleteNext(int index);
	int DeletePrev(int index);

	// 添加到头
	void AddHead(int id);
	// 添加到尾
	void AddTail(int id);

	// 删除头
	int DeleteHead();
	// 删除尾
	int DeleteTail();

	// 插入到pos处
	bool InsertPos(int id,int pos);
	// 删除pos处的项
	int DeletePos(int pos);
private:
	int head;
	int tail;
	struct link_node nodes[Max];
};
/************************************************************************/
/*                      初始化函数
/*                         Init
/************************************************************************/
template<unsigned int Max>
inline void DoubleLink<Max>::Init()
{
	head = -1;
	tail = -1;
}
/************************************************************************/
/*                      插入第一个元素
/*                        SetFirst
/************************************************************************/
template<unsigned int Max>
inline void DoubleLink<Max>::SetFirst(int id)
{
	head = id;
	tail = id;
	nodes[id].prev = id;
	nodes[id].next = id;
}
/************************************************************************/
/*                         正序查找
/*						ListTraverse
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::ListTraverse(int pos)
{
	int id = head;

	/* 检验参数是否合法 */ 
	if (pos < 0 || pos > Max || head == -1) return -1;

	/* 循环遍历 */
	for(int i=0;i<pos;i++)
        id = nodes[id].next;

	return id;
}
/************************************************************************/
/*                         逆序查找
/*						ListTraverseBack
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::ListTraverseBack(int pos)
{
	int id = tail;

	/* 检验参数是否合法 */ 
	if (pos < 0 || pos > Max || tail == -1) return -1;

	/* 循环遍历 */
	for(int i=0;i<pos;i++)
        id = nodes[id].prev;
	
	return id;
}
/************************************************************************/
/*                      插入到节点后面
/*                       InsertNext
/************************************************************************/
template<unsigned int Max>
inline bool DoubleLink<Max>::InsertNext(int id,int index)
{
	if (id >= Max || index >= Max || id < 0 || index < 0) return false;

	int new_prev = index;
	int new_next = nodes[index].next;
	
	nodes[id].prev = new_prev;
	nodes[id].next = new_next;
    nodes[new_prev].next = id;
    nodes[new_next].prev = id;
	return true;
}
/************************************************************************/
/*                      插入到节点前面
/*                       InsertPrev
/************************************************************************/
template<unsigned int Max>
inline bool DoubleLink<Max>::InsertPrev(int id,int index)
{
	if (id >= Max || index >= Max || id < 0 || index < 0) return false;

	int new_prev = nodes[index].prev;
	int new_next = index;
	
	nodes[id].prev = new_prev;
	nodes[id].next = new_next;
    nodes[new_prev].next = id;
    nodes[new_next].prev = id;
	return true;
}
/************************************************************************/
/*                   删除并返回下一个元素
/*                      DeleteNext
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeleteNext(int index)
{
	/* 检验参数是否合法 */ 
	if (index < 0 || index > Max) return -1;
	
	int new_prev = nodes[index].prev;
	int new_next = nodes[index].next;
	
	nodes[new_prev].next = new_next;
	nodes[new_next].prev = new_prev;

	return new_next;
}
/************************************************************************/
/*                   删除并返回上一个元素
/*                      DeletePrev
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeletePrev(int index)
{
	/* 检验参数是否合法 */ 
	if (index < 0 || index > Max) return -1;
	
	int new_prev = nodes[index].prev;
	int new_next = nodes[index].next;
	
	nodes[new_prev].next = new_next;
	nodes[new_next].prev = new_prev;

	return new_prev;
}
/************************************************************************/
/*                       添加到头
/*                       AddHead
/************************************************************************/
template<unsigned int Max>
inline void DoubleLink<Max>::AddHead(int id)
{
	if (head == -1)
	{
		SetFirst(id);
	}
	else
	{
		InsertPrev(id,head);
		head = id;
	}
}
/************************************************************************/
/*                       添加到尾
/*                        AddTail
/************************************************************************/
template<unsigned int Max>
inline void DoubleLink<Max>::AddTail(int id)
{
	if (head == -1)
	{
		SetFirst(id);
	}
	else
	{
		InsertNext(id,tail);
		tail = id;
	}
}
/************************************************************************/
/*                       删除头
/*                     DeleteHead
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeleteHead()
{
	int id = head;
	head = DeleteNext(head);
	return id;
}
/************************************************************************/
/*                       删除尾
/*                     DeleteTail
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeleteTail()
{
	int id = tail;
	tail = DeletePrev(tail);
	return id;
}

/************************************************************************/
/*                      插入到pos处
/*                      InsertPos
/************************************************************************/
template<unsigned int Max>
inline bool DoubleLink<Max>::InsertPos(int id,int pos)
{
	int index = ListTraverse(pos);
	if (index == -1) return false;
	InsertNext(id,index);
    return true;
}
/************************************************************************/
/*                     删除pos处的项
/*                      DeletePos
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeletePos(int pos)
{
	int index = ListTraverse(pos);
	if (index == -1) return -1;
	return DeleteNext(index);
}
#endif

