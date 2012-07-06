#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     ԭ�������� ����
 ===============================================================*/
//#include "Lock.h"


class Atomic
{
public:

	int Read();				/* �� */
	void Set(int Value);	/* д */

	void Add(int Value);	/* �� */
	void Sub(int Value);	/* �� */

	int SubTest(int Value);		/* ��������Ƿ�Ϊ0�����Ϊ0������1 */
	int AddNegative(int Value);	/* �Ӻ�����Ƿ�Ϊ�������Ϊ��������1 */

	/* �Լӻ��Լ����� */
	void Inc();		/* ��1 */
	void Dec();		/* ��1 */

	/* �ӻ���󣬲��Խ���Ƿ�Ϊ0�����Ϊ0���򷵻�1 */
	int IncTest();
	int DecTest();

	/* �ӻ���󣬷�����ֵ */
	int AddRet(int Value);
	int SubRet(int Value);
	int IncRet();
	int DecRet();

	/* λͼ���� */

	inline void SetBit(u8 Bit);
	inline void ClearBit(u8 Bit);
	inline bool TestBit(u8 Bit);

	volatile int value;
};
//#endif
