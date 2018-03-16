#ifndef _ERRNO_H_
#define	_ERRNO_H_

int errno;

/*
 * Error numbers.
 * TODO: Can't be sure of some of these assignments, I guessed from the
 * names given by strerror and the defines in the Cygnus errno.h. A lot
 * of the names from the Cygnus errno.h are not represented, and a few
 * of the descriptions returned by strerror do not obviously match
 * their error naming.
 */
#define EPERM		1	/* Operation not permitted */
#define	ENOFILE		2	/* No such file or directory */
#define	ESRCH		3	/* No such process */
#define	EINTR		4	/* Interrupted function call */
#define	EIO		5	/* Input/output error */
#define	ENXIO		6	/* No such device or address */
#define	E2BIG		7	/* Arg list too long */
#define	ENOEXEC		8	/* Exec format error */
#define	EBADF		9	/* Bad file descriptor */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Resource temporarily unavailable */
#define	ENOMEM		12	/* Not enough space */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
/* 15 - Unknown Error */
#define	EBUSY		16	/* strerror reports "Resource device" */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Improper link (cross-device link?) */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* Too many open files in system */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Inappropriate I/O control operation */
/* 26 - Unknown Error */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Invalid seek (seek on a pipe?) */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Domain error (math functions) */
#define	ERANGE		34	/* Result too large (possibly too small) */
/* 35 - Unknown Error */
#define	EDEADLOCK	36	/* Resource deadlock avoided (non-Cyg) */
#define	EDEADLK		36
/* 37 - Unknown Error */
#define	ENAMETOOLONG	38	/* Filename too long (91 in Cyg?) */
#define	ENOLCK		39	/* No locks available (46 in Cyg?) */
#define	ENOSYS		40	/* Function not implemented (88 in Cyg?) */
#define	ENOTEMPTY	41	/* Directory not empty (90 in Cyg?) */
#define	EILSEQ		42	/* Illegal byte sequence */

//Lista de errores (para no tener que poner todo en el perror()
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
	/*EXDEV 18*/ "Improper link ",
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
	/*ESPIPE 29*/ "Invalid seek ",
	/*EROFS 30*/ "Read-only file system ",
	/*EMLINK 31*/ "Too many links ",
	/*EPIPE 32*/ "Broken pipe ",
	/*EDOM 33*/ "Domain error ",
	/*ERANGE 34*/ "Result too large ",
	/*UNKWN 35*/ "Unknown error ",
	/*EDEADLOCK 36*/ "Resource deadlock avoided ",
	/*UNKWN 37*/ "Unknown error ",
	/*ENAMETOOLONG 38*/ "Filename too long ",
	/*ENOLCK 39*/ "No locks available ",
	/*ENOSYS 40*/ "Function not implemented ",
	/*ENOTEMPTY 41*/ "Directory not empty ",
	/*EILSEQ 42*/ "Illegal byte sequence "
};

#endif	/* Not _ERRNO_H_ */
