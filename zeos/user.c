#include <libc.h>
#include <sched.h>

char buff[24];

int pid;

long inner(long n)
{
	int i;
	long suma;
	suma = 0;
	for(i = 0; i < n; i++) suma = suma + i;
	return suma;
}

long outer(long n)
{
	int i;
	long acum;
	acum = 0;
	for(i = 0; i < n; i++) acum = acum + inner(i);
	return acum;
}

int add(int par1, int par2)
{
	return par1+par2;
}

int add2(int par1, int par2);

void printtime(int time)
{
	char buftime[256];
	itoa(time, buftime);
	write(1, buftime, strlen(buftime));
}

int __attribute__ ((__section__(".text.main")))

main(void)
{
	int time = gettime();
	printtime(time); //Prueba gettime

	write(1, "\nWelcome to ZeOS!\n", 18);	//Prueba write

	perror();	//Prueba perror

	init_task1();

	while(1);

	return 0;
}
