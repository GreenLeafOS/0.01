#ifndef BMPDATA_H
#define BMPDATA_H
/*===============================================================
 *                      BmpArray.h
 *                    数据结构-位图数组
 *													绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"
#include "Atomic.h"

#define MaxBmp (Max/32)+1

template <typename DataClass,unsigned int Max>
class BmpArray
{
public:
	void Init();

	Result GetFree(int &ID);

	Result Add(int &ID,DataClass Item);
	Result Add(DataClass Item);
	void Delete(int ID);

	Result Set(int ID,DataClass Item);
	Result Set(int ID,DataClass *Item);
	Result Get(int ID,DataClass &Item);
	Result GetAddr(int ID,DataClass **Item);
protected:
	Atomic bmp[MaxBmp];		// 位图
	DataClass data[Max];	// 数据
};
/************************************************************************/
/*                      初始化函数
/*                         Init
/************************************************************************/
template <typename DataClass,unsigned int Max>
void BmpArray<DataClass,Max>::Init()
{
	for(int i=0;i<MaxBmp;i++)
		bmp[i].Write(0);
	return;
}
/************************************************************************/
/*                       获取空闲ID
/*                       GetFree
/************************************************************************/
template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::GetFree(int &ID)
{
	for(int i=0;i<MaxBmp;i++)
		{
			Atomic byte = bmp[i];
			if (!(~(byte.Read()))) continue;
			for(int j=0;j<32;j++)
			{
				if (!(byte.TestBit(j)))
				{
					ID = (i*32)+j;
					return S_OK;
				}
			}
		}
	return E_MAX;
}
/************************************************************************/
/*                    	   添加
/*                         Add
/************************************************************************/
template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::Add(int &ID,DataClass Item)
{
	for(int i=0;i<MaxBmp;i++)
	{
		Atomic byte = bmp[i];
		if (!(~(byte.Read()))) continue;
		for(int j=0;j<32;j++)
		{
			if (!(byte.TestBit(j)))
			{
				ID = (i*32)+j;
				bmp[i].SetBit(j);
				this->data[ID] = Item;
				return S_OK;
			}
		}
	}
	return E_MAX;
}

template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::Add(DataClass Item)
{
	for(int i=0;i<MaxBmp;i++)
		{
			Atomic byte = bmp[i];
			if (!(~(byte.Read()))) continue;
			for(int j=0;j<32;j++)
			{
				if (!(byte.TestBit(j)))
				{
					int ID = (i*32)+j;
					bmp[i].SetBit(j);
					this->data[ID] = Item;
					return S_OK;
				}
			}
		}
	return E_MAX;
}
/************************************************************************/
/*                    	   删除项
/*                         Delete
/************************************************************************/
template <typename DataClass,unsigned int Max>
void BmpArray<DataClass,Max>::Delete(int ID)
{
	bmp[ID/32].ClearBit(ID%32);
	return;
}
/************************************************************************/
/*                    	   获取项
/*                          Get
/************************************************************************/
template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::Get(int ID,DataClass &Item)
{
	if(bmp[ID/32].TestBit(ID%32))
		Item = this->data[ID];
	else return E_NOITEM;
	return S_OK;
}

/************************************************************************/
/*                    	   设置项
/*                          Set
/************************************************************************/
template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::Set(int ID,DataClass Item)
{
	if(bmp[ID/32].TestBit(ID%32))
		this->data[ID] = Item;
	else return E_NOITEM;
	return S_OK;
}

template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::Set(int ID,DataClass *Item)
{
	if(bmp[ID/32].TestBit(ID%32))
		this->data[ID] = *Item;
	else return E_NOITEM;
	return S_OK;
}
/************************************************************************/
/*                    	  获取项指针
/*                        GetAddr
/************************************************************************/
template <typename DataClass,unsigned int Max>
Result BmpArray<DataClass,Max>::GetAddr(int ID,DataClass **Item)
{
	if(bmp[ID/32].TestBit(ID%32))
		*Item = &(this->data[ID]);
	else return E_NOITEM;
	return S_OK;
}
#endif
