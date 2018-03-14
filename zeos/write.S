#include <asm.h>

/*
Wrappers must carry out the following steps:
	1) Parameter passing: ZeOS implements the parameter passing from user mode to system
	mode through the CPU registers, as occurs in Linux. The parameters of the stack must be
	copied to the registers ebx, ecx, edx, esi, edi. The correct order is the first parameter (on the
	left) in ebx, the second parameter in ecx, etc.
	2) Put the identifier of the system call in the eax register (number 4 for write).
	3) Generate the trap: int $0x80.
	4) Process the result
	5) Return.
*/

ENTRY(write)
	push %ebp
	movl %esp, %ebp

	push %ebx 
	push %ecx
	push %edx

	movl 8(%ebp), %ebx //1a (int)
	movl 12(%ebp), %ecx //1b (puntero)
	movl 16(%ebp), %edx //1c (int)
	movl $4, %eax //2 
	int $0x80 //3 --> va al handler  --> ve nº4 --> va a la sys_call_table --> salta a sys_write
	cmpl $0, %eax //Miramos si el sys_write ha dado o no error
	jge fin
	imull $-1, %eax //Ponemos error en positivo para errno
	movl %eax, errno //4 (version con error)
	movl $-1, %eax

  fin:	pop %edx 
	pop %ecx
	pop %ebx
	pop %ebp //4 (version okey)
	ret //5
