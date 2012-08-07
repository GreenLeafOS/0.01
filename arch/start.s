.global		_start

.extern		gdt_init
.extern		idt_init
.extern		tss_init
.extern		init_8259A
.extern		mem_init
.extern		page_init

.extern		KernelMain

_start:
	call	gdt_init
	mov		$16,%ax
	mov		%ax,%ds
	mov		%ax,%es
	mov		%ax,%fs
	mov		$40,%ax
	mov		%ax,%gs

	call	init_8259A
	call	idt_init
	call	tss_init

	call	mem_init
	call	page_init

	jmp		KernelMain
