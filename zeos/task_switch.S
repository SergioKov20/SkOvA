#include <asm.h>

ENTRY(task_switch)
	push %ebp
	movl %esp, %ebp

	pushl %esi					//1 (guarda registros)
	pushl %edi
	pushl %ebx

	pushl 8(%ebp)			//Se pasa el parámetro union del task_switch

	call inner_task_switch_1	//2 (llama a la parte C de inner task switch)

	popl %ebx					//3 (restaura registros)
	popl %edi
	popl %esi

	movl %ebp, %esp
	popl %ebp
	ret


ENTRY(inner_task_switch_2) //Parte asm de inner_task_switch //TODO failea
	pushl %ebp

	movl 4(%ebp), %eax	//Guardar valor de EBP en el current task_struct
	movl %ebp, (%eax)

	movl 8(%ebp), %esp	//ESP apuntando al kernel esp del new task_struct

	pop %ebp
	ret
