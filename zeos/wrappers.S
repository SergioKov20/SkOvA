#include <asm.h>

/*
	Steps of a wrapper using sysenter:
	1)  Parameter passing. Similar to that in Section 3.8. (ebx - ecx - edx - esi - edi)
	2)  Put the identifier of the system call in the EAX register.
	3)  Store the values of ECX and EDX in the user stack.
	4)  Store the address of return to the user code in the user stack. This address 			corresponds to the assembler instruction following the sysenter instruction.
	5)  Create  a  fake  dynamic  link  to  access  previous  values  easily  from  the  			handler in  system mode.
	6)  Generate the trap: sysenter.
	7)  Pop EBP, the return address, EDX and ECX from the stack.
	8)  Process the result.
	9)  Return.
*/

ENTRY(write)
	push %ebp		//5
	movl %esp, %ebp

	push %ebx
	push %ecx		//3
	push %edx
	push %esi

	movl 8(%ebp), %ebx 	//1.1 (int, 4 bits)
	movl 12(%ebp), %ecx 	//1.2 (pointer, 4 bits)
	movl 16(%ebp), %edx	//1.3 (int, 4 bits)

	movl $4, %eax		//2

	leal backuser, %esi	//5

	sysenter		//6

backuser:	
	cmpl $0, %eax
	jge fin
	imul $-1, %eax		//pasar error positivo
	movl %eax, errno
	movl $-1, %eax		//valor de retorno -1

fin:
	pop %edx 			//7
	pop %ecx
	pop %ebx

	movl %ebp, %esp
	pop %ebp
	ret 				//9

	
ENTRY(gettime)
	push %ebp		
	movl %esp, %ebp

	push %ecx		
	push %edx
	push %esi

	movl $10, %eax		

	leal back_gettime, %esi	

	sysenter		

back_gettime:	
	cmpl $0, %eax
	jge fin_gettime
	imul $-1, %eax		
	movl %eax, errno
	movl $-1, %eax		

fin_gettime:
	movl %ebp, %esp
	pop %ebp
	ret 

ENTRY(getpid)
	push %ebp		
	movl %esp, %ebp

	push %ecx		
	push %edx
	push %esi

	movl $20, %eax		

	leal back_getpid, %esi	

	sysenter	

back_getpid:
	movl %ebp, %esp
	pop %ebp
	ret


ENTRY(fork) //TODO
	push %ebp		
	movl %esp, %ebp

	push %ecx		
	push %edx
	push %esi

	movl $2, %eax		

	leal back_fork, %esi	

	sysenter		

back_fork:	
	cmpl $0, %eax
	jge fin_fork
	imul $-1, %eax		
	movl %eax, errno
	movl $-1, %eax		

fin_fork:
	movl %ebp, %esp
	pop %ebp
	ret 			
