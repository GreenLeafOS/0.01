#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     原子数据类 定义
 ===============================================================*/
//#include "Lock.h"


class Atomic
{
public:

	int Read();				/* 读 */
	void Set(int Value);	/* 写 */

	void Add(int Value);	/* 加 */
	void Sub(int Value);	/* 减 */

	int SubTest(int Value);		/* 减后测试是否为0，如果为0，返回1 */
	int AddNegative(int Value);	/* 加后测试是否为负，如果为负，返回1 */

	/* 自加或自减运算 */
	void Inc();		/* 加1 */
	void Dec();		/* 减1 */

	/* 加或减后，测试结果是否为0，如果为0，则返回1 */
	int IncTest();
	int DecTest();

	/* 加或减后，返回新值 */
	int AddRet(int Value);
	int SubRet(int Value);
	int IncRet();
	int DecRet();

	/* 位图操作 */

	inline void SetBit(u8 Bit);
	inline void ClearBit(u8 Bit);
	inline bool TestBit(u8 Bit);

	volatile int value;
};
//#endif
