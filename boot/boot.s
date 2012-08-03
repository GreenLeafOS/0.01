/*===============================================================
 *                     	  boot.s
 *						������������
 ===============================================================*/
.global _start
.global read_loader
.code16
/************************************************************************/
/*                          ������
/*                          start
/************************************************************************/
jmp	_start
.org 128		/* Ԥ��128�ֽڵĿռ䣬���ڴ�ŷ�����ʲô�� */
_start:
	/* �������������� */
	mov %dl,(boot_drive)
	/* ��ʼ���μĴ�������ջָ�� */
	mov	%cs, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov $stack_top, %sp
	/* �Դ��es */
	mov 	$0xb800, %bx
	mov 	%bx, %es

	/* ����cls���� */
	call cls
	/* ����disp_str��ʾ�ַ��� */
	call disp_str
	/* ��ȡloader */
	call read_loader

	/* ��ѭ�� */
#	loop: jmp loop

	ljmp $0,$Loader_OffsetAddr
/************************************************************************/
/*                      	����
/*                          cls
/************************************************************************/
cls:
	mov	$0, %ax		/* ���ַ� */
	mov	$2000, %cx	/* ѭ��2000�� */
	xor	%di, %di	/* ��di��0 */
_cls_loop:
	mov	%ax, %es:(%di)	/* д�� */
	add	$2, %di			/* ָ����һ���ռ� */
	dec	%cx				/* ѭ����������1 */
	test %cx, %cx		/* �����Ƿ�Ϊ0 */
	jnz	_cls_loop
_cls_end:
	ret
/************************************************************************/
/*                      ����Ļ����ʾ�ַ���
/*                         disp_str
/************************************************************************/
disp_str:
	mov		$boot_msg, %si	/* �ַ�����ַ */
	mov		$0x0f, %ah		/* �ڵװ��� */
	xor		%di, %di		/* ��di��0 */
_disp_str_loop:
	lodsb					/* (C)al=*si;si++; */
	test	%al, %al		/* �����Ƿ�Ϊ0 */
	jz		_disp_str_end	/* Ϊ0����� */
	mov		%ax, %es:(%di)	/* д�� */
	add		$2,  %di		/* di+2*/
	jmp		_disp_str_loop	/* ������һ��ѭ�� */
_disp_str_end:
	ret
/************************************************************************/
/*                      ��ȡloader
/*                     read_loader
/************************************************************************/
Loader_Start		=	2
Loader_End			=	3
Loader_Size			=	Loader_End - Loader_Start + 1
Loader_BaseAddr		=	0
Loader_OffsetAddr	=	0x8000

read_loader:
	mov $Loader_BaseAddr,%ax	/* �λ�ַ */
	mov	%ax,%es
	mov	$Loader_OffsetAddr,%bx	/* ƫ���� */

	mov $2, %ah				/* �жϹ��ܺ� */
	mov	(boot_drive), %dl	/* ������ */
	mov	$0,	%dh				/* ��ͷ�� */
	mov	$0, %ch				/* ���� */
	mov	$Loader_Start,%cl	/* ��ʼ���� */
	mov	$Loader_Size,%al	/* ������ */
	int	$0x13				/* �����ж�*/

	jc	read_loader			/* ������ */
	ret
/************************************************************************/
/*                      	������
/*                          data
/************************************************************************/
boot_msg:
	.ascii	"Booting..."
boot_drive:
	.byte	0
free_space:
	.org	0x200-0x2
stack_top:
	.word	0xAA55
