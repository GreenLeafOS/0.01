#ifndef REG_H
#define REG_H
/*===============================================================
 *                      reg.h
 *                保护、恢复寄存器以及跳转
 ===============================================================*/

/*===========线程上下文寄存器结构=============*/
typedef struct stack_frame {
	/* PushSeg压入 */
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	/* PushReg压入*/
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	/* 系统压入 */
	u32	eip;
	u32	cs;
	u32	eflags;
	u32	esp;
	u32	ss;
}StackFrame;

/************************************************************************/
/*                     	通用寄存器
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
/*                     	段寄存器
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
/*                     	状态寄存器
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
/*                     	  跳转
/*                        Jmp
/************************************************************************/

/* 段内转移 */
inline void JmpNear(u32 eip)
{
	asm volatile("jmp *%0" : "=m"(eip));
}

/* 段间无特权级转移 */
inline void JmpFar(u32 cs,u32 eip)
{
	asm volatile("ljmp *%0,*%1" : "=m"(cs),"=m"(eip));
}

/* 段间转移。高特权级转移向低特权级 */
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
