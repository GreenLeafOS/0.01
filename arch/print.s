.section .bss
disp_pos:
		.long	0

.global disp_pos

.section .text
.global print

print:
	pushl	%ebp
	movl 	%esp,%ebp

	movl	8(%ebp),%esi
	movl	(disp_pos),%edi
	mov		$0x0F,%ah
_print_loop:
	lodsb
	test	%al,%al
	jz		_print_end
	cmp		$0x0A,%al
	jnz		_print_write
	pushl	%eax
	movl	%edi,%eax
	mov		$160,%bl
	div		%bl
	andl	$0x0FF,%eax
	incl	%eax
	mov		$160,%bl
	mul		%bl
	movl	%eax,%edi
	popl	%eax
	jmp		_print_loop
_print_write:
	mov		%ax,%gs:(%edi)
	addl	$2,%edi
	jmp		_print_loop

_print_end:
	movl	%edi,(disp_pos)

	popl	%ebp
	ret

