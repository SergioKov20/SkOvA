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

int sys_fork() //TODO
{
  	int PID=-1;

  	// creates the child process

	struct list_head *child_task_head = list_first(&freequeue); //1 get free task
	list_del(child_task_head);
	struct task_struct *child_task = list_head_to_task_struct(child_task_head);

	//2 TODO inherit system data: parent task_union you can use copy_data
	
	allocate_DIR(child_task);	//3

	//4 TODO alloc_frames(data+stack) if no se puede error

	//5 TODO todo lo demás RIP
  
  	return PID;
}

void sys_exit()
{  
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

