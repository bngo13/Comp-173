#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int* fibArr;

void *runner(void *param);

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "usage: %s <n>\n", argv[0]);
		return -1;
	}

	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%d argument must be >0", atoi(argv[1]));
		return -1;
	}

	int fibAmount = atoi(argv[1]);
	pthread_t thread; // Thread List
	pthread_attr_t attr; // Thread Attributes
	fibArr = malloc(sizeof(int) * fibAmount + 1);

	/* Default Values */
	fibArr[0] = 0;
	fibArr[1] = 1;

	/* Initialize Thread Attributes */
	pthread_attr_init(&attr);

	/* Start creating Threads */
	pthread_create(&thread, &attr, runner, &fibAmount);
	pthread_join(thread, NULL);


	/* Print out fibbo */
	for (int i = 0; i <= fibAmount; i++) {
		printf("%d ", fibArr[i]);
	}
	return 0;
}

void *runner(void *param) {
	int fib0 = fibArr[0];
	int fib1 = fibArr[1];

	int amount = *((int *)param);

	for (int i = 0; i < amount; i++) {
		int newSum = fib0 + fib1;
		fibArr[i + 2] = newSum;
		fib0 = fib1;
		fib1 = newSum;
	}
	pthread_exit(0);
}
