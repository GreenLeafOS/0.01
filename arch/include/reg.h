#ifndef REG_H
#define REG_H
/*===============================================================
 *                      reg.h
 *                �������ָ��Ĵ����Լ���ת
 ===============================================================*/

/*===========�߳������ļĴ����ṹ=============*/
typedef struct stack_frame {
	/* PushSegѹ�� */
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	/* PushRegѹ��*/
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	/* ϵͳѹ�� */
	u32	eip;
	u32	cs;
	u32	eflags;
	u32	esp;
	u32	ss;
}StackFrame;

/************************************************************************/
/*                     	ͨ�üĴ���
/*                        Reg
/************************************************************************/
#define PUSH_REG_OFFSET(reg_stack_frame) &reg_stack_frame.reg.eax
#define POP_REG_OFFSET(reg_stack_frame) &reg_stack_frame.reg.edi

inline void PushReg()
{
	asm volatile("pushal");
}

inline void PopReg()
{
	asm volatile("popal");
}


/************************************************************************/
/*                     	�μĴ���
/*                        Seg
/************************************************************************/
#define PUSH_SEG_OFFSET(reg_stack_frame) &reg_stack_frame.reg.ds
#define POP_SEG_OFFSET(reg_stack_frame) &reg_stack_frame.reg.gs

inline void PushSeg()
{
	asm volatile(
				"push %ds		\n"
				"push %es		\n"
				"push %fs		\n"
				"push %gs		\n"
				);
}

inline void PopSeg()
{
	asm volatile(
				"pop %gs		\n"
				"pop %fs		\n"
				"pop %es		\n"
				"pop %ds		\n"
				);
}


/************************************************************************/
/*                     	״̬�Ĵ���
/*                       Flags
/************************************************************************/
#define PUSH_FLAGS_OFFSET(reg_stack_frame) ((u32*)&reg_stack_frame.reg.eflags + 1)
#define POP_FLAGS_OFFSET(reg_stack_frame) &reg_stack_frame.reg.eflags

inline void PushFlags()
{
	asm volatile("pushf");
}

inline void PopFlags()
{
	asm volatile("popf");
}

/************************************************************************/
/*                     	  ��ת
/*                        Jmp
/************************************************************************/

/* ����ת�� */
inline void JmpNear(u32 eip)
{
	asm volatile("jmp *%0" : "=m"(eip));
}

/* �μ�����Ȩ��ת�� */
inline void JmpFar(u32 cs,u32 eip)
{
	asm volatile("ljmp *%0,*%1" : "=m"(cs),"=m"(eip));
}

/* �μ�ת�ơ�����Ȩ��ת�������Ȩ�� */
inline void JmpFar(u32 cs,u32 eip,u32 ss,u32 esp)
{
	asm volatile(
				"pushl	%0		\n"
				"pushl	%1		\n"
				"pushl	%2		\n"
				"pushl	%3		\n"
				"lret			\n"
				: "=m"(ss),"=m"(esp),"=m"(cs),"=m"(eip)
				);
}
#endif
