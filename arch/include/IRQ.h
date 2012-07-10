#ifndef IRQ_H_
#define IRQ_H_


/************************************************************************/
/*                      �ָ��Ĵ�������
/*                        Restart
/************************************************************************/
#define RestartNear							\
/*�ָ�ͨ�üĴ�����һЩ�μĴ���*/					\
asm volatile("cld 			\n"				\
			"pop %gs		\n"				\
			"pop %fs		\n"				\
			"pop %es		\n"				\
			"pop %ds		\n"				\
			"popal			\n"				\
			"ret"							\
			);

#define RestartFar 							\
/*�ָ�ͨ�üĴ�����һЩ�μĴ���*/					\
asm volatile("cld 			\n"				\
			"pop %gs		\n"				\
			"pop %fs		\n"				\
			"pop %es		\n"				\
			"pop %ds		\n"				\
			"popal			\n"				\
			"iretd"							\
			);

/************************************************************************/
/*                      ����Ĵ�������
/*                         Save
/************************************************************************/
#define Save								\
asm volatile("cld 			\n"				\
			"pushal			\n"				\
			"push %ds		\n"				\
			"push %es		\n"				\
			"push %fs		\n"				\
			"push %gs		\n"				\
			);


#endif /* IRQ_H_ */
