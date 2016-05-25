#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sema.h>

#define bool int
#define true 1
#define false 0

#define N_GRADS 6
#define N_UGRADS 2
#define N_STUDENTS (N_GRADS + N_UGRADS)
#define RUN_DURATION 10

// globals
pid_t processes[N_STUDENTS];
int grad_sem;
int ugrad_sems[N_UGRADS];
int table_sems[2];

int student_num; // needs to be global for be_killed()
int slices = 0; // needs to be global for be_killed()

void print_time() {
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("%d:%d ", timeinfo->tm_min, timeinfo->tm_sec);
}

void be_grad() {
	int table_num = student_num % 2;
	int table = table_sems[table_num];
	for (;;) { // infinite loop
		sem_down(grad_sem);
		sem_down(table); // get table
		print_time();
		printf("student %d (graduate)  at table %d \n", student_num, table_num);
		// usleep(200000); // eat
		slices++;

		for (int i = 0; i < N_UGRADS; i++)
			sem_up(ugrad_sems[i]);

		print_time();
		printf("student %d (graduate)  leaving table %d \n", student_num, table_num);
		sem_up(table); // leave table
		// usleep(400000); // go do other things
	}
}

void be_ugrad() {
	int ugrad_num = student_num - N_GRADS;
	int table_num = student_num % 2;
	int table = table_sems[table_num];
	for (;;) { // infinite loop
		for (int i = 0; i < N_GRADS; i++)
			sem_down(ugrad_sems[ugrad_num]);

		sem_down(table); // get table
		print_time();
		printf("student %d (undergrad) at table %d \n", student_num, table_num);
		// usleep(200000); // eat
		slices++;

		for (int i = 0; i < N_GRADS / N_UGRADS; i++)
			sem_up(grad_sem);

		// odd-number handling
		if ((N_GRADS % 2 + student_num % 2) == 2)
			sem_up(grad_sem);

		// coordinate with other undergrads
		for (int i = 0; i < N_UGRADS; i++)
			if (i != ugrad_num)
				sem_up(ugrad_sems[i]);
		sem_down(ugrad_sems[ugrad_num]);

		print_time();
		printf("student %d (undergrad) leaving table %d \n", student_num, table_num);
		sem_up(table); // leave table
		// sleep(400000); // go do other things
	}
}

// interrupt (SIGINT) handler
void be_killed() {
	print_time();
	printf("student %d ate %d slices \n", student_num, slices);
	exit(EXIT_SUCCESS);
}

pid_t spawn_student(bool grad, int num) {
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork error");
		exit(EXIT_FAILURE);
	} else if (pid == 0) {
		print_time();
		student_num = num;
		signal(SIGINT, be_killed);
		if (grad) {
			printf("NEW GRADUATE STUDENT \n");
			be_grad();
		} else {
			printf("NEW UNDERGRAD STUDENT \n");
			be_ugrad();
		}
		_exit(EXIT_SUCCESS);
	}
	return pid;
}

int main() {
	// make semaphores
	grad_sem = sem_init(N_GRADS);
	ugrad_sems[0] = sem_init(0);
	ugrad_sems[1] = sem_init(0);
	table_sems[0] = sem_init(1);
	table_sems[1] = sem_init(1);
	// make students
	for (int i = 0; i < N_STUDENTS; i++) {
		if (i < N_GRADS)
			processes[i] = spawn_student(true, i);
		else
			processes[i] = spawn_student(false, i);
	}
	// let students eat
	sleep(RUN_DURATION);
	// unmake students
	for (int i = 0; i < N_STUDENTS; i++) {
		kill(processes[i], SIGINT);
		waitpid(processes[i], NULL, 0);
	}
	// unmake semaphores
	int release_fail = 0;
	sem_release(grad_sem);
	sem_release(ugrad_sems[0]);
	sem_release(ugrad_sems[1]);
	sem_release(table_sems[0]);
	sem_release(table_sems[1]);

  return 0;
}
