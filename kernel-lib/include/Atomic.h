#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     ԭ�������� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "Type.h"

class Atomic
{
public:

	inline int Read();				/* �� */
	inline void Write(int Value);	/* д */

	inline void Add(int Value);		/* �� */
	inline void Sub(int Value);		/* �� */

	/* �Լӻ��Լ����� */
	inline void Inc();		/* ��1 */
	inline void Dec();		/* ��1 */

	/* �ӻ���󣬷�����ֵ */
	inline int AddRet(int Value);
	inline int SubRet(int Value);
	inline int IncRet();
	inline int DecRet();

	/* λͼ���� */

	inline void SetBit(u8 Bit);
	inline void ClearBit(u8 Bit);
	inline bool TestBit(u8 Bit);
private:
	volatile int value;
};
#endif
