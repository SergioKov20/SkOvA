#include <libc.h>

char buff[24];

int pid;

void print_var(unsigned long var)
{
	char buff[24];
	itoa(var, buff);
	write(1, buff, strlen(buff));
	write(1, "\n", 1);
}

void workload_1()
{
	struct stats st;

	int x = read(0, 20, 2000);
	x = get_stats(pid, &st);

	write(1, "\nWorkload 1:\n", 13);
	
	if(x != -1) {
		print_var(st.elapsed_total_ticks);
		print_var(st.user_ticks);
		print_var(st.blocked_ticks);
		print_var(st.ready_ticks);
		print_var(st.system_ticks);
	}
}

void workload_2()
{

}

void workload_3()
{

}

int __attribute__ ((__section__(".text.main")))

main(void)
{	
	int err = set_sched_policy(0); //0 RR , 1 FCFS
	if(err == -1) {
		itoa(err, buff);
		write(1, buff, 4);
		write(1, "\n", 1);
	}
	workload_1();

	while(1);

	return 0;
}
