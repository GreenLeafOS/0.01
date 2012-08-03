/*===============================================================
 *                     	  boot.s
 *						启动扇区代码
 ===============================================================*/
.global _start
.global read_loader
.code16
/************************************************************************/
/*                          主函数
/*                          start
/************************************************************************/
jmp	_start
.org 128		/* 预留128字节的空间，用于存放分区表什么的 */
_start:
	/* 保存启动驱动器 */
	mov %dl,(boot_drive)
	/* 初始化段寄存器及堆栈指针 */
	mov	%cs, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov $stack_top, %sp
	/* 显存段es */
	mov 	$0xb800, %bx
	mov 	%bx, %es

	/* 调用cls清屏 */
	call cls
	/* 调用disp_str显示字符串 */
	call disp_str
	/* 读取loader */
	call read_loader

	/* 死循环 */
#	loop: jmp loop

	ljmp $0,$Loader_OffsetAddr
/************************************************************************/
/*                      	清屏
/*                          cls
/************************************************************************/
cls:
	mov	$0, %ax		/* 空字符 */
	mov	$2000, %cx	/* 循环2000次 */
	xor	%di, %di	/* 对di清0 */
_cls_loop:
	mov	%ax, %es:(%di)	/* 写入 */
	add	$2, %di			/* 指向下一个空间 */
	dec	%cx				/* 循环计数器减1 */
	test %cx, %cx		/* 测试是否为0 */
	jnz	_cls_loop
_cls_end:
	ret
/************************************************************************/
/*                      在屏幕上显示字符串
/*                         disp_str
/************************************************************************/
disp_str:
	mov		$boot_msg, %si	/* 字符串地址 */
	mov		$0x0f, %ah		/* 黑底白字 */
	xor		%di, %di		/* 对di清0 */
_disp_str_loop:
	lodsb					/* (C)al=*si;si++; */
	test	%al, %al		/* 测试是否为0 */
	jz		_disp_str_end	/* 为0则结束 */
	mov		%ax, %es:(%di)	/* 写入 */
	add		$2,  %di		/* di+2*/
	jmp		_disp_str_loop	/* 进入下一次循环 */
_disp_str_end:
	ret
/************************************************************************/
/*                      读取loader
/*                     read_loader
/************************************************************************/
Loader_Start		=	2
Loader_End			=	3
Loader_Size			=	Loader_End - Loader_Start + 1
Loader_BaseAddr		=	0
Loader_OffsetAddr	=	0x8000

read_loader:
	mov $Loader_BaseAddr,%ax	/* 段基址 */
	mov	%ax,%es
	mov	$Loader_OffsetAddr,%bx	/* 偏移量 */

	mov $2, %ah				/* 中断功能号 */
	mov	(boot_drive), %dl	/* 驱动器 */
	mov	$0,	%dh				/* 磁头号 */
	mov	$0, %ch				/* 柱面 */
	mov	$Loader_Start,%cl	/* 起始扇区 */
	mov	$Loader_Size,%al	/* 扇区数 */
	int	$0x13				/* 调用中断*/

	jc	read_loader			/* 检测错误 */
	ret
/************************************************************************/
/*                      	数据区
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
