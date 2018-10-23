/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <system.h>

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int ret_from_fork()
{
	return 0;
}

int sys_fork()
{
  	int PID = -1;

  	// creates the child process

	if(list_empty(&freequeue)) return PID; //1 get free task, error if no space
	
	struct list_head *child_task_head = list_first(&freequeue); 
	list_del(child_task_head);
	struct task_struct *child_task = list_head_to_task_struct(child_task_head);

	union task_union *child_task_union = (union task_union *)child_task; //2 inherit parent task_union + you can use copy_data to copy
	union task_union *parent_task_union = (union task_union *)current();
	copy_data(parent_task_union, child_task_union, sizeof(union task_union));
	
	allocate_DIR(child_task);	//3

	int frames[NUM_PAG_DATA]; //4 check available frames + not enough free pages = error
	for(int i = 0; i < NUM_PAG_DATA; i++) {
		frames[i] = alloc_frame();
		if(frames[i] == -1) { //no hay frames suficientes, toca liberar
			for(int j = 0; j < i; j++) free_frame(frames[j]);
			return -1;
		}
	}

	//5 Inherit user data:

	page_table_entry *tp_child = get_PT(child_task);
	page_table_entry *tp_parent = get_PT(current());

	for(int i = 0; i < PAG_LOG_INIT_DATA; i++) {			
		set_ss_pag(tp_child, i, get_frame(tp_parent, i));
	}

	int start = PAG_LOG_INIT_DATA + NUM_PAG_DATA;
	int temp = -1;
	while(start < TOTAL_PAGES && temp == -1) {
		if(tp_parent[start].entry == 0) temp = start;
		start++;
	}

	if(temp == -1) return PID;

	void *child_pageaddress;
	void *parent_pageaddress;

	for(int i = 0; i < NUM_PAG_DATA; i++)
	{
		set_ss_pag(tp_child, PAG_LOG_INIT_DATA + i, frames[i]);
		set_ss_pag(tp_parent, temp, frames[i]);

		child_pageaddress = (void *) ((temp) * PAGE_SIZE);
		parent_pageaddress = (void *) ((PAG_LOG_INIT_DATA + i) * PAGE_SIZE);

		copy_data(parent_pageaddress, child_pageaddress, PAGE_SIZE);
		del_ss_pag(tp_parent, temp);
		set_cr3(get_DIR(&parent_task_union->task));
	}

	//new PID
	child_task->PID = get_newPID();
	tp_parent[temp].entry = 0;

	child_task->kernel_esp = (unsigned long) &child_task_union->stack[KERNEL_STACK_SIZE - 19];
	child_task_union->stack[KERNEL_STACK_SIZE - 19] = 0;
	child_task_union->stack[KERNEL_STACK_SIZE - 18] = (unsigned long) &ret_from_fork;

	list_add_tail(&child_task->list, &readyqueue);

	//Esto es del planificador
	child_task->state = ST_READY;
	child_task->quantum = 3;

  	return child_task->PID;
}

void sys_exit()
{ 
	int frames[NUM_PAG_DATA];
	for(int i = 0; i < NUM_PAG_DATA; i++)
	{
		free_frame(frames[i]);
	} 
	list_del(&(current()->list));
	list_add_tail(&(current()->list), &freequeue);
}

//SYS_WRITE
/*
1) Check  the  user  parameters
	a)  Check  the  fd with check_fd, If  the  operation  indicated  for  this  file
		descriptor is right, the function returns 0. Otherwise, it returns a negative identifier of
		the generated error.
	b)  Check buffer. In this case, it will be enough to verify that the pointer parameter is not
		NULL.
	c)  Check size. Check positive values.

2) Copy  the  data  from/to  the  user  address  space  if  needed.  See  the  functions copy_to_user and copy_from_user (section 3.8.7).

3)Implement the requested service. For the I/O system calls, this requires calling the device
dependent routine. In this particular case, the device dependent routine is sys_write_console

4)Return  the  result */

int sys_write(int fd, char * buffer, int size)
{
	int error;

	error = check_fd(fd, ESCRIPTURA);
	if(error != 0) return error; 	//1a) EACCESS or BAD FILE DESCRIPTOR (-9 o -13)
	if(buffer == NULL) return -14; 	//1b) INVALID ADDRESS
	if(size < 0) return -22; 	//1c) INVALID ARGUMENT

	char sysbuffer[size]; //Para copiar el buffer del modo usuario
	error = copy_from_user(buffer, sysbuffer, size); //2, si da error se retorna -1
	if(error != 0) return error;

	sys_write_console(sysbuffer, size); //3

	return 0; //4
}

//SYS_GETTIME

int sys_gettime()
{
	return zeos_ticks;
}

