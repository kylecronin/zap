	.cstring
LC0:
	.ascii "sup, world!\0"
	.text
.globl _main
_main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	L3
"L00000000001$pb":
L3:
	popl	%ebx
	leal	LC0-"L00000000001$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	L_puts$stub
	movl	$0, %eax
	addl	$20, %esp
	popl	%ebx
	leave
	ret
	.section __IMPORT,__jump_table,symbol_stubs,self_modifying_code+pure_instructions,5
L_puts$stub:
	.indirect_symbol _puts
	hlt ; hlt ; hlt ; hlt ; hlt
	.subsections_via_symbols
