.global		_start

.extern		gdt_init
.extern		idt_init
.extern		tss_init
.extern		init_8259A
.extern		buddy_init

.extern		kernel_main

.section 	.text
_start:
	call	gdt_init
	mov		$16,%ax
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs
	mov		%ax,%ss

	mov		$40,%ax
	mov		%ax,%gs

	movl	$0x80000,%esp

	call	init_8259A
	call	idt_init
	call	tss_init

	call	buddy_init
#	call	page_init

	jmp		kernel_main
