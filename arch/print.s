.section .bss
disp_pos:
		.long	0



.section .text
.global print

print:
	push	%ebp
	mov	 	%esp,%ebp

	mov		8(%ebp),%esi
	mov		(disp_pos),%edi
	mov		$0x0F,%ah
_print_loop:
	lodsb
	test	%al,%al
	jz		_print_end
	cmp		$0x0A,%al
	jnz		_print_write
	push	%eax
	mov		%edi,%eax
	mov		$160,%bl
	div		%bl
	and		$0x0FF,%eax
	inc		%eax
	mov		$160,%bl
	mul		%bl
	mov		%eax,%edi
	pop		%eax
	jmp		_print_loop
_print_write:
	mov		%ax,%gs:(%edi)
	add		$2,%edi
	jmp		_print_loop

_print_end:
	mov		%edi,(disp_pos)

	pop		%ebp
	ret
