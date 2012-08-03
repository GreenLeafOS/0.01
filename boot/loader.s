/*===============================================================
 *                     	 loader.s
 *						引导程序代码
 ===============================================================*/
.global	_start

.include "gdt.s"

Kernel_Start		=	3
Kernel_End			=	18
Kernel_Size			=	Kernel_End - Kernel_Start + 1
Kernel_FileAddr		=	0x4000		/* 暂时可以随便放 */
Kernel_OffSetAddr	=	0x10000		/* 1M开始 */
/************************************************************************/
/*						入口函数
/*						start
/************************************************************************/
.code16
_start:
	/* 初始化 */
	mov	%cs, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss
	mov	$0x7000,%sp

	/* 读入内核 */
	mov	$0, %ax
	mov	%ax,%es
	mov	$Kernel_FileAddr, %bx
	mov	$Kernel_Start, %al
	mov	$Kernel_Size,  %cl
	call read_sector

	/* 跳入保护模式 */
	jmp	jmp_pm

/************************************************************************/
/*						读取磁盘
/*					  read_sector
/* 入口参数：	al 起始扇区
/*			cl 扇区数
	读入 es:bx 中
/************************************************************************/
read_sector:
	push	%bp
	mov		%sp,%bp
	sub		$2, %sp			/* 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2] */
	mov		%cl,-2(%bp)
	push	%bx				/* 保存 bx */
	mov		$18,%bl			/* bl: 除数 */
	div		%bl				/* y 在 al 中, z 在 ah 中 */
	inc		%ah				/* z ++ */
	mov		%ah,%cl 		/* cl <- 起始扇区号 */
	mov		%al,%dh 		/* dh <- y */
	shr		$1,%al			/* y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2) */
	mov		%al,%ch			/* ch <- 柱面号 */
	and		$1,%dh			/* dh & 1 = 磁头号 */
 	pop		%bx				/* 恢复 bx */
_read_sector_reading:
	mov		$2,%ah			/* 读 */
	mov		-2(%bp),%al		/* 读 al 个扇区 */
	int		$0x13
	jc		_read_sector_reading	/* 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止 */
	add		$2,%sp
	pop		%bp

	ret
/************************************************************************/
/*                      GDT段描述符表
/*                         gdt
/************************************************************************/
.code16
gdt:
/*标签				宏名			段基址		段界限		属性							*/
gdt_null:			Descriptor	0,			0,			0
gdt_desc_code:		Descriptor	0,			0xfffff,	DA_CR  | DA_32 | DA_LIMIT_4K
gdt_desc_data:		Descriptor	0,			0xfffff,	DA_DRW | DA_32 | DA_LIMIT_4K
gdt_desc_video:		Descriptor	0xb8000,	0x0ffff,	DA_DRW | DA_DPL3

/* gdtptr */
gdt_len		=	gdt_ptr - gdt
gdt_ptr:
	.word	gdt_len - 1		/* 段界限 */
	.word	gdt				/* 基地址低位 */
	.word	0				/* 高位一定为0 */

/* gdt选择子 */
sel_code		=	gdt_desc_code	- gdt
sel_data		=	gdt_desc_data	- gdt
sel_video		=	gdt_desc_video	- gdt + SA_RPL3
/************************************************************************/
/*                     	跳入保护模式
/*                       jmp_pm
/************************************************************************/
jmp_pm:
	/* 加载gdt */
	lgdt	(gdt_ptr)
	/* 关中断 */
	cli

	/* 打开地址线A20 */
	in		$0x92,%al
	or		$0x02, %al	/* 二进制数00000010 */
	out		%al,$0x92

	/* 准备切换到保护模式,修改cr0寄存器 */
	movl	%cr0,%eax
	or		$1,%eax
	mov		%eax,%cr0

/* 真正进入保护模式 */
jmp32:
	/* AT&T汇编格式  ljmp $sel_code,pm_start */
	/* Intel汇编格式 jmp far dword sel_code:pm_start */
	/* 机器代码 */
	.quad 0x000800008200ea66
/************************************************************************/
/*                 		   保护模式初始化
/*                         	pm_start
/************************************************************************/
.code32
.org 512
pm_start:
	mov		$sel_data,%bx
	mov		%bx,%ds
	mov		%bx,%es
	mov		%bx,%ss
	mov		%bx,%fs
	mov		$0x8000,%esp

	mov 	$sel_video, %bx
	mov 	%bx, %gs			/* 显存段gs */
/************************************************************************/
/*                      初始化并进入内核(跳入保护模式后)
/*                        goto_kernel
/************************************************************************/
Elf_phnum		=	Kernel_FileAddr + 0x2c
Elf_phoff		=	Kernel_FileAddr + 0x1c

goto_kernel:
	mov		(Elf_phnum),%ecx			/* ecx:Program header table 中条目数 */
	movzx	%cx,%ecx
	movl	(Elf_phoff),%esi			/* esi:Program header table 的偏移量 */
	addl	$Kernel_FileAddr,%esi		/* 加上基址，esi指向 Program header table */
_goto_kernel_begin:
	movl	0(%esi),%eax				/* eax:p_type 段类型 */
	cmpl	$0,%eax						/* 如果eax为0 */
	jz		_goto_kernel_noaction		/* 跳入下一条目 */
	pushl	0x10(%esi)					/* 压入 p_vaddr 虚拟地址(目的指针) */
	movl	0x04(%esi),%eax				/* eax:p_offset 程序段在文件中的偏移 */
	addl	$Kernel_FileAddr,%eax		/* 加上基址，eax指向程序段 */
	pushl	%eax						/* 压入程序段地址(源指针) */
	pushl	0x08(%esi)					/* 压入 p_vaddr 虚拟地址(目的指针) */
	call	memcpy						/* 调用memcpy复制内存 */
	addl	$12,%esp					/* 清理堆栈 */
_goto_kernel_noaction:
	addl	$0x20,%esi					/* esi指向下一个条目 */
	dec		%ecx						/* 条目计数减1 */
	jnz		_goto_kernel_begin
_goto_kernel:
	ljmp	$sel_code,$Kernel_OffSetAddr
/************************************************************************/
/*                        内存拷贝
/*                        memcpy
/* void* memcpy(void* es:pDest, void* ds:pSrc, int iSize);
/************************************************************************/
.code32
memcpy:
	push	%ebp
	mov		%esp,%ebp

	push	%esi
	push	%edi
	push	%ecx

	mov		8(%ebp),%edi	/* Destination */
	mov		12(%ebp),%esi	/* Source */
	mov		16(%ebp),%ecx	/* Counter */
_memcpy_copy:
	cmp		$0, %ecx 		/* 判断计数器 */
	jz		_memcpy_end		/* 计数器为零时跳出 */

	mov		%ds:(%esi),%al	/* 读取 */
	inc		%esi

	mov		%al,%es:(%edi)	/* 写入 */
	inc		%edi

	dec		%ecx			/* 计数器减一 */
	jmp		_memcpy_copy	/* 循环 */
_memcpy_end:
	mov		8(%ebp),%eax 	/* 返回值 */

	pop		%ecx
	pop		%edi
	pop		%esi
	mov		%ebp,%esp
	pop		%ebp

	ret						/* 函数结束，返回 */

