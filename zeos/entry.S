/*
 * entry.S - Entry point to system mode from user mode
 */

#include <asm.h>
#include <segment.h>


/**************************************************/
/**** Save & Restore ******************************/
/**                                              **/
/** When we change to privilege level 0 (kernel) **/
/** (through an interrupt, a system call, an     **/
/** exception ...) we must save the state of the **/
/** currently running task (save).               **/
/**                                              **/
/** Stack layout in 'systemCall':                **/
/**                                              **/
/**   0(%esp) - %ebx    \                        **/
/**   4(%esp) - %ecx     |                       **/
/**   8(%esp) - %edx     |                       **/
/**   C(%esp) - %esi     | Register saved        **/
/**  10(%esp) - %edi     |  by 'save'            **/
/**  14(%esp) - %ebp     |                       **/
/**  18(%esp) - %eax     |                       **/
/**  1C(%esp) - %ds      |                       **/
/**  20(%esp) - %es      |                       **/
/**  24(%esp) - %fs      |                       **/
/**  28(%esp) - %gs     /                        **/
/**  2C(%esp) - %eip    \                        **/
/**  30(%esp) - %cs      |                       **/
/**  34(%esp) - %eflags  |  Return context saved **/
/**  38(%esp) - %oldesp  |   by the processor.   **/
/**  3C(%esp) - %oldss  /                        **/
/**                                              **/
/**************************************************/

#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %edx; \
      pushl %ecx; \
      pushl %ebx; \
      movl $__KERNEL_DS, %edx;    \
      movl %edx, %ds;           \
      movl %edx, %es

#define RESTORE_ALL \
	popl %ebx; \
	popl %ecx; \
	popl %edx; \
	popl %esi; \
	popl %edi; \
	popl %ebp; \
	popl %eax; \
	popl %ds; \
	popl %es; \
	popl %fs; \
	popl %gs

//End Of Interruption
#define EOI \
	movb $0x20, %al ; \
	outb %al, $0x20 ;

/*Recordar posar la capçalera void dels handlers a include/entry.h!!
	Capçalera ENTRY
	1) SAVE ALL (guardar contexto)
	2) EOI
	3) call service_routine
	4) RESTORE_ALL (restore contexto)
	-- (opcional) clear del error code si tiene --
	5) iret (no ret porque no es un return normal, tiene que cambiar de mode) */

ENTRY(keyboard_handler)
	SAVE_ALL
	EOI
	call keyboard_routine
	RESTORE_ALL	
	iret

ENTRY(clock_handler)
	SAVE_ALL
	EOI
	call clock_routine
	RESTORE_ALL
	iret

ENTRY(system_call_handler)
	SAVE_ALL
	cmpl $0, %eax				//Syscall negativo = error
	jl err
	cmpl $MAX_SYSCALL, %eax			//Syscall mayor de 4 = error
	jg err
	call *sys_call_table(, %eax, 0x04)	//Saltamos a la rutina de servicio
	jmp fin					//Fin
  err:
	movl $-ENOSYS, %eax			//Ponemos el error ENOSYS
  fin:
	movl %eax, 0x18(%esp)			//Cambio del valor de %eax en la pila
	RESTORE_ALL
	iret
