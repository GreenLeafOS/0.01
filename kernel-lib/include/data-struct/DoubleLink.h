#ifndef DOUBLELINK_H
#define DOUBLELINK_H
/*===============================================================
 *                      DoubleLink.h
 *                    ���ݽṹ-˫������
 *													��Ҷ-��������
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
	
	// �����һ��Ԫ��
	void SetFirst(int id);

	// �������
	int ListTraverse(int pos);
	// �������
	int ListTraverseBack(int pos);
	// ���
	bool InsertPrev(int id,int index);
	bool InsertNext(int id,int index);
	// ɾ��
	int DeleteNext(int index);
	int DeletePrev(int index);

	// ��ӵ�ͷ
	void AddHead(int id);
	// ��ӵ�β
	void AddTail(int id);

	// ɾ��ͷ
	int DeleteHead();
	// ɾ��β
	int DeleteTail();

	// ���뵽pos��
	bool InsertPos(int id,int pos);
	// ɾ��pos������
	int DeletePos(int pos);
private:
	int head;
	int tail;
	struct link_node nodes[Max];
};
/************************************************************************/
/*                      ��ʼ������
/*                         Init
/************************************************************************/
template<unsigned int Max>
inline void DoubleLink<Max>::Init()
{
	head = -1;
	tail = -1;
}
/************************************************************************/
/*                      �����һ��Ԫ��
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
/*                         �������
/*						ListTraverse
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::ListTraverse(int pos)
{
	int id = head;

	/* ��������Ƿ�Ϸ� */ 
	if (pos < 0 || pos > Max || head == -1) return -1;

	/* ѭ������ */
	for(int i=0;i<pos;i++)
        id = nodes[id].next;

	return id;
}
/************************************************************************/
/*                         �������
/*						ListTraverseBack
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::ListTraverseBack(int pos)
{
	int id = tail;

	/* ��������Ƿ�Ϸ� */ 
	if (pos < 0 || pos > Max || tail == -1) return -1;

	/* ѭ������ */
	for(int i=0;i<pos;i++)
        id = nodes[id].prev;
	
	return id;
}
/************************************************************************/
/*                      ���뵽�ڵ����
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
/*                      ���뵽�ڵ�ǰ��
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
/*                   ɾ����������һ��Ԫ��
/*                      DeleteNext
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeleteNext(int index)
{
	/* ��������Ƿ�Ϸ� */ 
	if (index < 0 || index > Max) return -1;
	
	int new_prev = nodes[index].prev;
	int new_next = nodes[index].next;
	
	nodes[new_prev].next = new_next;
	nodes[new_next].prev = new_prev;

	return new_next;
}
/************************************************************************/
/*                   ɾ����������һ��Ԫ��
/*                      DeletePrev
/************************************************************************/
template<unsigned int Max>
inline int DoubleLink<Max>::DeletePrev(int index)
{
	/* ��������Ƿ�Ϸ� */ 
	if (index < 0 || index > Max) return -1;
	
	int new_prev = nodes[index].prev;
	int new_next = nodes[index].next;
	
	nodes[new_prev].next = new_next;
	nodes[new_next].prev = new_prev;

	return new_prev;
}
/************************************************************************/
/*                       ��ӵ�ͷ
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
/*                       ��ӵ�β
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
/*                       ɾ��ͷ
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
/*                       ɾ��β
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
/*                      ���뵽pos��
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
/*                     ɾ��pos������
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

