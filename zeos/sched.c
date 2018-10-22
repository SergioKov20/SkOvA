/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}

struct list_head freequeue;
struct list_head readyqueue;

struct task_struct *idle_task;
struct task_struct *new_task;

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
	struct list_head *first_free_task = list_first(&freequeue); 	//Coge el primero libre de la freequeue (list_first en list.h)
	list_del(first_free_task); 					//Como va a cambiar se quita de la freequeue
	idle_task = list_head_to_task_struct(first_free_task);		//Tenemos un list_head pero queremos el task_struct

	idle_task->PID = 0;						//Punto 2 PDF
	allocate_DIR(idle_task); 					//Punto 3 PDF

	/* Ayuda de Alex en el lab para idle: 
	"task_struct *t apunta al task_struct (y al task_union porque apuntan al mismo sitio)
	Yo puedo hacer un casting u = ((union task_union*) t) para que apunte a la task_union.
	Convirtiendo la t de task_struct a task_union puedo acceder a los campos de la t.union.
	Ahora puedo hacer un u->stack[..] para acceder en alto nivel a la pila de sistema." */

	union task_union* idle_task_union = (union task_union*) idle_task;
	//Context switch:
	idle_task_union->stack[KERNEL_STACK_SIZE - 1] = &cpu_idle;	//Dirección a ejecutar = cpu_idle
	idle_task_union->stack[KERNEL_STACK_SIZE - 2] = 0;		//Valor inicial registro ebp post dynamic link

	//Guardar en campo struct que contiene el union la posición de pila del valor inicial anterior
	idle_task_union->task.kernel_esp = &(idle_task_union->stack[KERNEL_STACK_SIZE - 2]);
	
}

void init_task1(void)
{
	struct list_head *first_free_task = list_first(&freequeue);
	list_del(first_free_task);
	new_task = list_head_to_task_struct(first_free_task); 	// 0) Coger proceso libre + conversión a task struct

	new_task->PID = 1;										// 1) PID = 1
	allocate_DIR(new_task);									// 2) AllocateDIR
	set_user_pages(new_task);								// 3) set_user_pages par inicializar espacio de dir.

	// 4) Modificar stack pointer del TSS para apuntar a la pila de sistema de new_task + Modificado WriteMSR 0x175
	union task_union* new_task_union = (union task_union*) new_task;
	tss.esp0 = &new_task_union->stack[KERNEL_STACK_SIZE];
	writeMSR(0x175, tss.esp0);

	set_cr3(get_DIR(new_task));								// 5) set_cr3 para poner directorio páginas como el actual
}


void init_sched()
{
	//init freequeue (INIT_LIST_HEAD en list.c inicializa lista vacía)
	INIT_LIST_HEAD(&freequeue);
	int i;	
	for(i = 0; i < NR_TASKS; i++) //Añadir todos los NR_TASKS procesos a la cola de ready)
	{ 
		list_add(&(task[i].task.list), &freequeue);
	}

	//init readyqueue
	INIT_LIST_HEAD(&readyqueue);
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void inner_task_switch_1(union task_union *new_union) //Misma cabecera que task_switch / parte C de inner
{
	//1 Modificar MSR 0x175 para que apunte a la pila de new_union)
	tss.esp0 = new_union->task.kernel_esp;
	writeMSR(0x175, tss.esp0);

	set_cr3(get_DIR(&new_union->task)); //2 Cambiar espacio de direcciones de usuario por el actual

	inner_task_switch_2(&current()->kernel_esp, new_union->task.kernel_esp);	//3 se pasan las pilas de sistema (actual y a cambiar)
}
