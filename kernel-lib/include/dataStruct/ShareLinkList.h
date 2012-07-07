#ifndef SHARELINKLIST_H_
#define SHARELINKLIST_H_
/*===============================================================
 *                     ShareLinkList.h
 *                 ���ݽṹ-�������ݻ��������
 *													��Ҷ-��������
 ===============================================================*/
#include "Type.h"
#include "BmpArray.h"

// ������
template<typename DataClass>
typedef struct link_item
{
	DataClass item;
	u16 id;
	u16 next;
}LinkList;

// �������ݻ��������
// ֻ�ܲ���ͷβ���ݵ�����
template<typename DataClass,unsigned int Max>
class ShareLinkList
{
public:
	inline void Init();

	inline Result AddHead(DataClass Item);
	inline Result AddTail(DataClass Item);

	inline Result DeleteHead(DataClass &Item);
	inline Result DeleteHead();

	// �����α�����ĺ���
	Result Insert(DataClass Item);
	Result Delete();
	Result Set(DataClass Item);
	Result Set(DataClass *Item);
	Result Get(DataClass &Item);
	Result GetAddr(DataClass **Item);
	// �α꺯��
	inline void MoveHead();
	inline void MoveTail();
	inline Result MoveNext();
protected:
	static BmpArray<LinkList<DataClass>,Max> data;	// ���ݻ�����
private:
	u16 head;
	u16 tail;

	u16 last_cursor;
	u16 cursor;
};
/************************************************************************/
/*                       ��ʼ������
/*                         Init
/************************************************************************/
template <typename DataClass,unsigned int Max>
inline void ShareLinkList<DataClass,Max>::Init()
{
	head = 0;
	tail = 0;
	last_cursor = 0;
	cursor = 0;
	return;
}
/************************************************************************/
/*                    	  �����ͷ��
/*                        AddHead
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::AddHead(DataClass Item)
{
	LinkList<DataClass> newone;

	// ��ȡID
	if (data.GetFree(newone.id) != S_OK) return E_MAX;
	// ������
	newone.item = Item;
	// ���뵽ͷ��
	newone.next = head;
	head = newone.id;

	data.Set(newone.id,newone);	// д��

	return S_OK;
}
/************************************************************************/
/*                    	  �����β��
/*                         AddTail
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::AddTail(DataClass Item)
{
	LinkList<DataClass> newone;
	LinkList<DataClass> *ptail;

	// ��ȡID
	if (data.GetFree(newone.id) != S_OK) return E_MAX;	// ��ȡID
	// ������
	newone.item = Item;

	// ���뵽β��
	if (data.GetAddr(tail,&ptail) == S_OK);	// ���βָ�����
		ptail->next = newone.id;			// ����βָ���nextָ��������
	newone.next = 0;			// ����next
	tail = newone.id;			// βָ����Ϊ��ֵ

	// д��
	data.Set(newone.id,newone);

	return S_OK;
}
/************************************************************************/
/*                    	  ɾ��ͷ������
/*                        DeleteHead
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::DeleteHead(DataClass &Item)
{
	// ��ȡβָ��
	LinkList<DataClass> *phead;
	if (data.GetAddr(tail,&phead) != S_OK) return E_EMPTY;
	// ͷָ��ָ����һ��
	head = phead->next;
	// ����ɾ������
	Item = phead->item;
	// �ڻ�������ɾ��
	data.Delete(phead->id);

	return S_OK;
}

template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::DeleteHead()
{
	// ��ȡβָ��
	LinkList<DataClass> *phead;
	if (data.GetAddr(tail,&phead) != S_OK) return E_EMPTY;
	// ͷָ��ָ����һ��
	head = phead->next;
	// �ڻ�������ɾ��
	data.Delete(phead->id);

	return S_OK;
}

/************************************************************************/
/*                    	    ����
/*                         Insert
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Insert(DataClass Item)
{
	LinkList<DataClass> *plast;
	LinkList<DataClass> *pnext;
	LinkList<DataClass> newone;
	if (data.GetFree(newone.id) != S_OK) return E_MAX;

	// �������һ��������һ�����next��Ϊnewid
	if (data.GetAddr(last_cursor,plast) == S_OK)
	{
		plast->next = newone.id;
	}
	// ����α괦�����������next��Ϊpnext��id
	if (data.GetAddr(cursor,pnext) == S_OK)
	{
		newone.next	= pnext->id;
	}
	return S_OK;
}

/************************************************************************/
/*                    	    ɾ��
/*                         Delete
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Delete(DataClass Item)
{
	LinkList<DataClass> *plast;
	LinkList<DataClass> *pnext;

	// �������һ����͵�ǰ��
	if (  (data.GetAddr(last_cursor,plast) == S_OK) && (data.GetAddr(cursor,pnext) == S_OK)  )
	{
		plast->next = pnext->id;
		data.Delete(cursor);
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}

/************************************************************************/
/*                    	    ����
/*                          Set
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Set(DataClass Item)
{
	LinkList<DataClass> *pitem;
	// ����α괦�����������item����
	if (data.GetAddr(cursor,pitem) == S_OK)
	{
		pitem->item = Item;
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}

template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Set(DataClass *Item)
{
	LinkList<DataClass> *pitem;
	// ����α괦�����������item����
	if (data.GetAddr(cursor,pitem) == S_OK)
	{
		pitem->item = *Item;
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}

/************************************************************************/
/*                    	    ��ȡ
/*                          Get
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Get(DataClass &Item)
{
	LinkList<DataClass> *pitem;
	// ����α괦�����������item����
	if (data.GetAddr(cursor,pitem) == S_OK)
	{
		 Item = pitem->item;
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}

/************************************************************************/
/*                    	    ��ȡ��ָ��
/*                           GetAddr
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::GetAddr(DataClass **Item)
{
	LinkList<DataClass> *pitem;
	// ����α괦�����������item����
	if (data.GetAddr(cursor,pitem) == S_OK)
	{
		 *Item = &pitem->item;
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}

/************************************************************************/
/*                    	   �ƶ��α�
/*                          Move...
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline void ShareLinkList<DataClass,Max>::MoveHead()
{
	cursor = head;
	return;
}

template<typename DataClass,unsigned int Max>
inline void ShareLinkList<DataClass,Max>::MoveTail()
{
	cursor = tail;
	return;
}

template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::MoveNext()
{
	LinkList<DataClass> *pitem;
	// ����α괦�����ȡnext
	if (data.GetAddr(cursor,pitem) == S_OK)
	{
		last_cursor = cursor;
		cursor = pitem->next;
	}
	else
	{
		return E_NOITEM;
	}
	return S_OK;
}
#endif /* SHARELINKLIST_H_ */
