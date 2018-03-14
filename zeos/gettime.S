#include <asm.h>

entry(gettime)
	push %ebp
	movl %esp, %ebp
	int $0x80
	
