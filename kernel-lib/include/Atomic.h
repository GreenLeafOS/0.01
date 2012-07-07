#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     原子数据类 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"

class Atomic
{
public:

	inline int Read();				/* 读 */
	inline void Write(int Value);	/* 写 */

	inline void Add(int Value);		/* 加 */
	inline void Sub(int Value);		/* 减 */

	/* 自加或自减运算 */
	inline void Inc();		/* 加1 */
	inline void Dec();		/* 减1 */

	/* 加或减后，返回新值 */
	inline int AddRet(int Value);
	inline int SubRet(int Value);
	inline int IncRet();
	inline int DecRet();

	/* 位图操作 */

	inline void SetBit(u8 Bit);
	inline void ClearBit(u8 Bit);
	inline bool TestBit(u8 Bit);
private:
	volatile int value;
};
#endif
