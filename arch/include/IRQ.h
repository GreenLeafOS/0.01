#ifndef IRQ_H_
#define IRQ_H_


/************************************************************************/
/*                      »Ö¸´¼Ä´æÆ÷º¯Êý
/*                        Restart
/************************************************************************/
#define RestartNear							\
/*»Ö¸´Í¨ÓÃ¼Ä´æÆ÷ºÍÒ»Ð©¶Î¼Ä´æÆ÷*/					\
asm volatile("cld 			\n"				\
			"pop %gs		\n"				\
			"pop %fs		\n"				\
			"pop %es		\n"				\
			"pop %ds		\n"				\
			"popal			\n"				\
			"ret"							\
			);

#define RestartFar 							\
/*»Ö¸´Í¨ÓÃ¼Ä´æÆ÷ºÍÒ»Ð©¶Î¼Ä´æÆ÷*/					\
asm volatile("cld 			\n"				\
			"pop %gs		\n"				\
			"pop %fs		\n"				\
			"pop %es		\n"				\
			"pop %ds		\n"				\
			"popal			\n"				\
			"iretd"							\
			);

/************************************************************************/
/*                      ±£´æ¼Ä´æÆ÷º¯Êý
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
