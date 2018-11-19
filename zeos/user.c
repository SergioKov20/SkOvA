#include <libc.h>

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

void workload_1()
{

}

void workload_2()
{

}

void workload_3()
{

}

void workload_4()
{

}

int __attribute__ ((__section__(".text.main")))

main(void)
{

	while(1);

	return 0;
}
