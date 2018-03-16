#include <asm.h>

//gettime --> int --> handler --> syscalltable --> sys_gettime

ENTRY(gettime)
	push %ebp
	movl %esp, %ebp
	movl $10, %eax //Para que vaya a la entrada 10 de la syscalltable
	int $0x80
	cmpl $0, %eax
	jge fin
	imull $-1, %eax
	movl %eax, errno
	movl $-1, %eax	

  fin:	
	pop %ebp
	ret
