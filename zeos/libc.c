/*
 * libc.c 
 */

#include <libc.h>
#include <errno.h>
#include <types.h>

int errno;

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

void perror()
{
	char *lista_errores[] = {
		/*EPERM 1*/ "Operation not permitted",
		/*ENOFILE 2*/ "No such file or directory",
		/*ESRCH 3*/ "No such process", 
		/*EINTR 4*/ "Interrupted function call ", 
		/*EIO 5*/  "Input/output error ", 
		/*ENXIO 6*/  "No such device or address ",
		/*E2BIG 7*/  "Arg list too long ",
		/*ENOEXEC 8*/  "Exec format error ",
		/*EBADF 9*/ "Bad file descriptor ",
		/*ECHILD 10*/ "No child processes ",
		/*EAGAIN 11*/ "Resource temporarily unavailable ",
		/*ENOMEM 12*/ "Not enough space ",
		/*EACCES 13*/ "Permission denied ",
		/*EFAULT 14*/ "Bad address ",
		/*UNKWN 15*/ "Unknown error ",
		/*EBUSY 16*/ "strerror reports 'Resource device' ",
		/*EEXIST 17*/ "File exists ",
		/*EXDEV 18*/ "Improper link (cross-device link?) ",
		/*ENODEV 19*/ "No such device ",
		/*ENOTDIR 20*/ "Not a directory ",
		/*EISDIR 21*/ "Is a directory ",
		/*EINVAL 22*/ "Invalid argument ",
		/*ENFILE 23*/ "Too many open files in system ",
		/*EMFILE 24*/ "Too many open files ",
		/*ENOTTY 25*/ "Inappropriate I/O control operation ",
		/*UNKWN 26*/ "Unknown error ",
		/*EFBIG 27*/ "File too large ",
		/*ENOSPC 28*/ "No space left on device ",
		/*ESPIPE 29*/ "Invalid seek (seek on a pipe?) ",
		/*EROFS 30*/ "Read-only file system ",
		/*EMLINK 31*/ "Too many links ",
		/*EPIPE 32*/ "Broken pipe ",
		/*EDOM 33*/ "Domain error (math functions) ",
		/*ERANGE 34*/ "Result too large (possibly too small) ",
		/*UNKWN 35*/ "Unknown error ",
		/*EDEADLOCK 36*/ "Resource deadlock avoided (non-Cyg) ",
		/*UNKWN 37*/ "Unknown error ",
		/*ENAMETOOLONG 38*/ "Filename too long (91 in Cyg?) ",
		/*ENOLCK 39*/ "No locks available (46 in Cyg?) ",
		/*ENOSYS 40*/ "Function not implemented (88 in Cyg?) ",
		/*ENOTEMPTY 41*/ "Directory not empty (90 in Cyg?) ",
		/*EILSEQ 42*/ "Illegal byte sequence "
	}; //TODO: PREGUNTAR ESTO 

	char *errormsg;
	if (errno == 0) errormsg = "No error";
	else if (errno < 0 || errno > 42) errormsg = "Unknown error";
	else errormsg = lista_errores[errno-1];
	write (1, errormsg, strlen(errormsg));
	write (1, "\n", strlen(errormsg));

} 
