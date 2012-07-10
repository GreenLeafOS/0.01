#ifndef SHARELINKLIST_H_
#define SHARELINKLIST_H_
/*===============================================================
 *                     ShareLinkList.h
 *                 数据结构-共享数据缓冲的链表
 *													绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"
#include "BmpArray.h"

// 链表项
template<typename DataClass>
struct link_item
{
	DataClass item;
	u16 id;
	u16 next;
};

// 共享数据缓冲的链表
// 只能操作头尾数据的链表
template<typename DataClass,unsigned int Max>
class ShareLinkList
{
public:
	inline void Init();

	inline Result AddHead(DataClass Item);
	inline Result AddTail(DataClass Item);

	inline Result DeleteHead(DataClass &Item);
	inline Result DeleteHead();

	// 根据ID操作的函数
	Result Get(int ID,DataClass &Item);
	Result GetAddr(int ID,DataClass **Item);
	Result Set(int ID,DataClass Item);
	Result Set(int ID,DataClass *Item);

	// 根据游标操作的函数
	Result Insert(DataClass Item);
	Result Delete();
	Result Set(DataClass Item);
	Result Set(DataClass *Item);
	Result Get(DataClass &Item);
	Result GetAddr(DataClass **Item);

	// 游标函数
	inline void MoveHead();
	inline void MoveTail();
	inline Result MoveNext();

	inline BmpArray<struct link_item<DataClass>,Max> *GetStaticAddr();


public:
	u16 head;
	u16 tail;

	u16 last_cursor;
	u16 cursor;
protected:
	static BmpArray<struct link_item<DataClass>,Max> data;	// 数据缓冲区
};
/************************************************************************/
/*                       初始化函数
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
/*                    	  添加在头部
/*                        AddHead
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::AddHead(DataClass Item)
{
	struct link_item<DataClass> newone;

	// 获取ID
	if (data.GetFree(newone.id) != S_OK) return E_MAX;
	// 复制项
	newone.item = Item;
	// 插入到头部
	newone.next = head;
	head = newone.id;

	data.Set(newone.id,newone);	// 写入

	return S_OK;
}
/************************************************************************/
/*                    	  添加在尾部
/*                         AddTail
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::AddTail(DataClass Item)
{
	struct link_item<DataClass> newone;
	struct link_item<DataClass> *ptail;

	// 获取ID
	if (data.GetFree(newone.id) != S_OK) return E_MAX;	// 获取ID
	// 复制项
	newone.item = Item;

	// 插入到尾部
	if (data.GetAddr(tail,&ptail) == S_OK);	// 如果尾指针存在
		ptail->next = newone.id;			// 设置尾指针的next指向加入的项
	newone.next = 0;			// 重置next
	tail = newone.id;			// 尾指针设为新值

	// 写入
	data.Set(newone.id,newone);

	return S_OK;
}
/************************************************************************/
/*                    	  删除头部的项
/*                        DeleteHead
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::DeleteHead(DataClass &Item)
{
	// 获取尾指针
	struct link_item<DataClass> *phead;
	if (data.GetAddr(tail,&phead) != S_OK) return E_EMPTY;
	// 头指针指向下一个
	head = phead->next;
	// 返回删除的项
	Item = phead->item;
	// 在缓冲区中删除
	data.Delete(phead->id);

	return S_OK;
}

template<typename DataClass,unsigned int Max>
inline Result ShareLinkList<DataClass,Max>::DeleteHead()
{
	// 获取尾指针
	struct link_item<DataClass> *phead;
	if (data.GetAddr(tail,&phead) != S_OK) return E_EMPTY;
	// 头指针指向下一个
	head = phead->next;
	// 在缓冲区中删除
	data.Delete(phead->id);

	return S_OK;
}

/************************************************************************/
/*                    	    插入
/*                         Insert
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Insert(DataClass Item)
{
	struct link_item<DataClass> *plast;
	struct link_item<DataClass> *pnext;
	struct link_item<DataClass> newone;
	if (data.GetFree(newone.id) != S_OK) return E_MAX;

	// 如果有上一个项，则把上一个项的next设为newid
	if (data.GetAddr(last_cursor,plast) == S_OK)
	{
		plast->next = newone.id;
	}
	// 如果游标处有项，则把新项的next设为pnext的id
	if (data.GetAddr(cursor,pnext) == S_OK)
	{
		newone.next	= pnext->id;
	}
	return S_OK;
}

/************************************************************************/
/*                    	    删除
/*                         Delete
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Delete(DataClass Item)
{
	struct link_item<DataClass> *plast;
	struct link_item<DataClass> *pnext;

	// 如果有上一个项和当前项
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
/*                    	    设置
/*                          Set
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Set(DataClass Item)
{
	struct link_item<DataClass> *pitem;
	// 如果游标处有项，则把新项的item更新
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
	struct link_item<DataClass> *pitem;
	// 如果游标处有项，则把新项的item更新
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
/*                    	    获取
/*                          Get
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Get(DataClass &Item)
{
	struct link_item<DataClass> *pitem;
	// 如果游标处有项，则把新项的item更新
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
/*                    	    获取项指针
/*                           GetAddr
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::GetAddr(DataClass **Item)
{
	struct link_item<DataClass> *pitem;
	// 如果游标处有项，则把新项的item更新
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
/*                    	   移动游标
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
	struct link_item<DataClass> *pitem;
	// 如果游标处有项，获取next
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
/************************************************************************/
/*                    	 	根据ID操作的函数
/*                         Get GetAddr Set
/************************************************************************/
template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Get(int ID,DataClass &Item)
{
	struct link_item<DataClass> *link;
	if (data.GetAddr(ID,&link) != S_OK) return E_NOITEM;
	Item = link->item;
	return S_OK;
}


template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::GetAddr(int ID,DataClass **Item)
{
	struct link_item<DataClass> *link;
	if (data.GetAddr(ID,&link) != S_OK) return E_NOITEM;

	*Item = &link->item;
	return S_OK;
}

template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Set(int ID,DataClass Item)
{
	struct link_item<DataClass> *link;
	if (data.GetAddr(ID,&link) != S_OK) return E_NOITEM;

	link->item = Item;
	return S_OK;
}

template<typename DataClass,unsigned int Max>
Result ShareLinkList<DataClass,Max>::Set(int ID,DataClass *Item)
{
	struct link_item<DataClass> *link;
	if (data.GetAddr(ID,&link) != S_OK) return E_NOITEM;

	link->item = *Item;
	return S_OK;
}

/************************************************************************/
/*                    	 获取静态数据缓冲区指针
/*                          GetStaticAddr
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline BmpArray<struct link_item<DataClass>,Max>* ShareLinkList<DataClass,Max>::GetStaticAddr()
{
	return &data;
}
#endif /* SHARELINKLIST_H_ */
