/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

#include <errno.h>

#include <system.h>

extern int zeos_ticks; //Para compartir el valor en interrupt

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

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}


/* SYS WRITE
	1) Check the user parameters: fd, buffer and size. Bear in mind that the system has to be
	robust and assume that the parameters from the user space are unsafe by default (lib.c is
	user code).
		a) Check the fd, we will use a new int check_fd (int fd, int operation) function that
		checks whether the specified file descriptor and requested operation are correct. The
		operations can be LECTURA or ESCRIPTURA. If the operation indicated for this file
		descriptor is right, the function returns 0. Otherwise, it returns a negative identifier of
		the generated error.
		b) Check buffer. In this case, it will be enough to verify that the pointer parameter is not
		NULL.
		32
		c) Check size. Check positive values.
	2) Copy the data from/to the user address space if needed. See the functions copy_to_user
	and copy_from_user (section 3.7.7).
	3) Implement the requested service. For the I/O system calls, this requires calling the device
	dependent routine. In this particular case, the device dependent routine is sys_write_console:
	int sys_write_console (char *buffer, int size);
	This function displays size bytes contained in the buffer and returns the number of bytes
	written.
	4) Return the result.
*/

int sys_write(int fd, char * buffer, int size)
{
	int ret;

	//Punto 1 (a/b/c)
	ret = check_fd(fd, ESCRIPTURA);
	if (ret != 0) return ret; //ERROR: PERMISOS (EACCES) o BAD FILE DESCRIPTOR
	if (buffer == NULL) return -14; //ERROR: INVALID ADREÇA (-14)
	if (size < 0) return -22; //ERROR: ARGUMENT INVALID (-22)

	//Punto 2 y 3 (optimizado, basta con saber si devuelve bytes error y devolverlos)
	char local_buffer[size];
	ret = copy_from_user(buffer, local_buffer, size);
	if(ret != 0) return ret; //Luego será negativo y dará unknown error	

	sys_write_console(local_buffer, size);

	return 0; //Punto 4
}

int sys_gettime() 
{
	return zeos_ticks;
}
