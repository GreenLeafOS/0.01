#ifndef ARRAY_H_
#define ARRAY_H_
/*===============================================================
 *                     StackArray.h
 *                    数据结构-栈数组
 *													绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"

template<typename DataClass,unsigned int Max>
class StackArray
{
public:
	inline void Init();

	inline Result Add(int &ID,DataClass Item);
	inline Result Add(DataClass Item);

	inline Result Delete();

	inline Result Set(int ID,DataClass Item);
	inline Result Get(int ID,DataClass &Item);
	inline Result GetAddr(int ID,DataClass **Item);
protected:
	int count;
	DataClass data[Max];	// 数据
};
/************************************************************************/
/*                      初始化函数
/*                         Init
/************************************************************************/
template <typename DataClass,unsigned int Max>
inline void StackArray<DataClass,Max>::Init()
{
	count = 0;
	return;
}
/************************************************************************/
/*                    	   添加
/*                         Add
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::Add(int &ID,DataClass Item)
{
	ID = count;
	if (count++ == Max) return E_MAX;
	data[ID] = Item;
	return S_OK;
}

template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::Add(DataClass Item)
{
	int ID = count;
	if (count++ == Max) return E_MAX;
	data[ID] = Item;
	return S_OK;
}
/************************************************************************/
/*                    		删除项
/*                          Delete
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::Delete()
{
	if (count) 	count--;
	else 		return E_EMPTY;

	return S_OK;
}
/************************************************************************/
/*                    		获取项
/*                           Get
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::Get(int ID,DataClass &Item)
{
	if(ID >= count) return E_MAX;
	Item = data[ID];
	return S_OK;
}
/************************************************************************/
/*                    		设置项
/*                           Set
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::Set(int ID,DataClass Item)
{
	if(ID >= count) return E_MAX;
	data[ID] = Item;
	return S_OK;
}
/************************************************************************/
/*                    	  获取项指针
/*                        GetAddr
/************************************************************************/
template<typename DataClass,unsigned int Max>
inline Result StackArray<DataClass,Max>::GetAddr(int ID,DataClass **Item)
{
	if(ID >= count) return E_MAX;
	*Item = &data[ID];
	return S_OK;
}

#endif /* ARRAY_H_ */
