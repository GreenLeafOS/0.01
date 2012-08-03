/*===============================================================
 *                     	 loader.s
 *						�����������
 ===============================================================*/
.global	_start

.include "gdt.s"

Kernel_Start		=	3
Kernel_End			=	18
Kernel_Size			=	Kernel_End - Kernel_Start + 1
Kernel_FileAddr		=	0x4000		/* ��ʱ�������� */
Kernel_OffSetAddr	=	0x10000		/* 1M��ʼ */
/************************************************************************/
/*						��ں���
/*						start
/************************************************************************/
.code16
_start:
	/* ��ʼ�� */
	mov	%cs, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %ss
	mov	$0x7000,%sp

	/* �����ں� */
	mov	$0, %ax
	mov	%ax,%es
	mov	$Kernel_FileAddr, %bx
	mov	$Kernel_Start, %al
	mov	$Kernel_Size,  %cl
	call read_sector

	/* ���뱣��ģʽ */
	jmp	jmp_pm

/************************************************************************/
/*						��ȡ����
/*					  read_sector
/* ��ڲ�����	al ��ʼ����
/*			cl ������
	���� es:bx ��
/************************************************************************/
read_sector:
	push	%bp
	mov		%sp,%bp
	sub		$2, %sp			/* �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2] */
	mov		%cl,-2(%bp)
	push	%bx				/* ���� bx */
	mov		$18,%bl			/* bl: ���� */
	div		%bl				/* y �� al ��, z �� ah �� */
	inc		%ah				/* z ++ */
	mov		%ah,%cl 		/* cl <- ��ʼ������ */
	mov		%al,%dh 		/* dh <- y */
	shr		$1,%al			/* y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2) */
	mov		%al,%ch			/* ch <- ����� */
	and		$1,%dh			/* dh & 1 = ��ͷ�� */
 	pop		%bx				/* �ָ� bx */
_read_sector_reading:
	mov		$2,%ah			/* �� */
	mov		-2(%bp),%al		/* �� al ������ */
	int		$0x13
	jc		_read_sector_reading	/* �����ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ */
	add		$2,%sp
	pop		%bp

	ret
/************************************************************************/
/*                      GDT����������
/*                         gdt
/************************************************************************/
.code16
gdt:
/*��ǩ				����			�λ�ַ		�ν���		����							*/
gdt_null:			Descriptor	0,			0,			0
gdt_desc_code:		Descriptor	0,			0xfffff,	DA_CR  | DA_32 | DA_LIMIT_4K
gdt_desc_data:		Descriptor	0,			0xfffff,	DA_DRW | DA_32 | DA_LIMIT_4K
gdt_desc_video:		Descriptor	0xb8000,	0x0ffff,	DA_DRW | DA_DPL3

/* gdtptr */
gdt_len		=	gdt_ptr - gdt
gdt_ptr:
	.word	gdt_len - 1		/* �ν��� */
	.word	gdt				/* ����ַ��λ */
	.word	0				/* ��λһ��Ϊ0 */

/* gdtѡ���� */
sel_code		=	gdt_desc_code	- gdt
sel_data		=	gdt_desc_data	- gdt
sel_video		=	gdt_desc_video	- gdt + SA_RPL3
/************************************************************************/
/*                     	���뱣��ģʽ
/*                       jmp_pm
/************************************************************************/
jmp_pm:
	/* ����gdt */
	lgdt	(gdt_ptr)
	/* ���ж� */
	cli

	/* �򿪵�ַ��A20 */
	in		$0x92,%al
	or		$0x02, %al	/* ��������00000010 */
	out		%al,$0x92

	/* ׼���л�������ģʽ,�޸�cr0�Ĵ��� */
	movl	%cr0,%eax
	or		$1,%eax
	mov		%eax,%cr0

/* �������뱣��ģʽ */
jmp32:
	/* AT&T����ʽ  ljmp $sel_code,pm_start */
	/* Intel����ʽ jmp far dword sel_code:pm_start */
	/* �������� */
	.quad 0x000800008200ea66
/************************************************************************/
/*                 		   ����ģʽ��ʼ��
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
	mov 	%bx, %gs			/* �Դ��gs */
/************************************************************************/
/*                      ��ʼ���������ں�(���뱣��ģʽ��)
/*                        goto_kernel
/************************************************************************/
Elf_phnum		=	Kernel_FileAddr + 0x2c
Elf_phoff		=	Kernel_FileAddr + 0x1c

goto_kernel:
	mov		(Elf_phnum),%ecx			/* ecx:Program header table ����Ŀ�� */
	movzx	%cx,%ecx
	movl	(Elf_phoff),%esi			/* esi:Program header table ��ƫ���� */
	addl	$Kernel_FileAddr,%esi		/* ���ϻ�ַ��esiָ�� Program header table */
_goto_kernel_begin:
	movl	0(%esi),%eax				/* eax:p_type ������ */
	cmpl	$0,%eax						/* ���eaxΪ0 */
	jz		_goto_kernel_noaction		/* ������һ��Ŀ */
	pushl	0x10(%esi)					/* ѹ�� p_vaddr �����ַ(Ŀ��ָ��) */
	movl	0x04(%esi),%eax				/* eax:p_offset ��������ļ��е�ƫ�� */
	addl	$Kernel_FileAddr,%eax		/* ���ϻ�ַ��eaxָ������ */
	pushl	%eax						/* ѹ�����ε�ַ(Դָ��) */
	pushl	0x08(%esi)					/* ѹ�� p_vaddr �����ַ(Ŀ��ָ��) */
	call	memcpy						/* ����memcpy�����ڴ� */
	addl	$12,%esp					/* �����ջ */
_goto_kernel_noaction:
	addl	$0x20,%esi					/* esiָ����һ����Ŀ */
	dec		%ecx						/* ��Ŀ������1 */
	jnz		_goto_kernel_begin
_goto_kernel:
	ljmp	$sel_code,$Kernel_OffSetAddr
/************************************************************************/
/*                        �ڴ濽��
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
	cmp		$0, %ecx 		/* �жϼ����� */
	jz		_memcpy_end		/* ������Ϊ��ʱ���� */

	mov		%ds:(%esi),%al	/* ��ȡ */
	inc		%esi

	mov		%al,%es:(%edi)	/* д�� */
	inc		%edi

	dec		%ecx			/* ��������һ */
	jmp		_memcpy_copy	/* ѭ�� */
_memcpy_end:
	mov		8(%ebp),%eax 	/* ����ֵ */

	pop		%ecx
	pop		%edi
	pop		%esi
	mov		%ebp,%esp
	pop		%ebp

	ret						/* �������������� */

