ENTRY(_start)
SECTIONS
{
	.text 0x100000 : AT(200) 
	{ 
		*(.text .data .rodata .bss) *(COMMON)
	}
	.memory 0x30000 : AT(0)
	{
		*(memory)
	}
	.shstrtab : AT(0)
	{
		*(.shstrtab)
	}
	 /DISCARD/ : 
	 {
	 	*(.symtab) *(.comment) *(.eh_frame) *(.iplt) *(.shstrtab)  *(.strtab) *(.shstrtab) *(.rel.dyn)
	 }
}
