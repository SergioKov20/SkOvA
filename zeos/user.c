#include <libc.h>

char buff[24];

int pid;

int fib(int n)
{
  if (n == 0 || n == 1)
    return n;
  else
    return (fib(n-1) + fib(n-2));
}

void print_stats(int procpid, int workload) //Esta función muestra todos los ticks y tiempo del proceso
{
	struct stats st;
	get_stats(procpid, &st);

	itoa(procpid, buff);	//Info del proceso
	write(1, "Proc", 5);
	write(1, buff, strlen(buff));
	write(1, " / Workload", 11);
	itoa(workload, buff);
	write(1, buff, strlen(buff));
	write(1, "\n", 1);


	itoa(st.user_ticks, buff);	//Stats del proceso
	write(1, "User ", 5);
	write(1, buff, strlen(buff));
	write(1, " // ", 4);

	itoa(st.blocked_ticks, buff);
	write(1, "Blocked ", 8);
	write(1,buff, strlen(buff));
	write(1, " // ", 4);

	itoa(st.ready_ticks, buff);
	write(1, "Ready ", 6);
	write(1, buff, strlen(buff));
	write(1, " // ", 4);

	itoa(st.system_ticks, buff);
	write(1, "System ", 7);
	write(1, buff, strlen(buff));
	write(1, " // ", 4);


	int totaltime = st.user_ticks + st.blocked_ticks + st.ready_ticks + st.system_ticks;
	itoa(totaltime, buff);	//Tiempo total
	write(1,"Total time: ", 12);
	write(1, buff, strlen(buff));
	write(1, "\n", 1);
}

/* Enunciado workloads: you  can  test  several  combinations:  
all processes  being  CPU  bounded (1),  all  processes  being  I/O  bounded (2),  combination  of  both  types  of
processes (3) ...*/

void workload_1()
{
	int p1, p2;
	
	p1 = fork();
	if(p1 == 0) 
	{
		for(int i = 15; i <= 30; i++) fib(i);
		print_stats(getpid(), 1);
		exit();
	}

	p2 = fork();
	if(p2 == 0) 
	{
		for(int i = 15; i <= 30; i++) fib(i);
		print_stats(getpid(), 1);
		exit();
	}
	exit();
}

void workload_2()
{
	int p1, p2;
	
	p1 = fork();
	if(p1 == 0) 
	{
		read(0, buff, 1000);
		print_stats(getpid(), 2);
		exit();
	}

	p2 = fork();
	if(p2 == 0) 
	{
		read(0, buff, 1000);
		print_stats(getpid(), 2);
		exit();
	}
	exit();
}

void workload_3()
{
	int p1, p2;
	
	p1 = fork();
	if(p1 == 0) 
	{
		for(int i = 15; i <= 30; i++) fib(i);
		read(0, buff, 1000);
		print_stats(getpid(), 1);
		exit();
	}

	p2 = fork();
	if(p2 == 0) 
	{
		for(int i = 15; i <= 30; i++) fib(i);
		read(0, buff, 1000);
		print_stats(getpid(), 1);
		exit();
	}
	exit();
}

int __attribute__ ((__section__(".text.main")))

main(void)
{	
	int err = set_sched_policy(1); //0 RR , 1 FCFS
	if(err == -1) {
		itoa(err, buff);
		write(1, buff, 4);
		write(1, "\n", 1);
	}
	workload_1();
	//workload_2();
	//workload_3();

	while(1);

	return 0;
}
