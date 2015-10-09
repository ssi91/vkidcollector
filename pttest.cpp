#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static void wait_thread(void)
{
	time_t start_time = time(NULL);

	while (time(NULL) == start_time)
	{
		/* do nothing except chew CPU slices for up to one second. */
	}
}

static void *thread_func(void *vptr_args)
{
	int i;

	for (i = 0; i < 20; i++)
	{
		fputs("  b\n", stderr);
		wait_thread();
	}

	return NULL;
}

int main(void)
{
	int i;
	pthread_t thread;
	pthread_t thread1;

	if (pthread_create(&thread, NULL, thread_func, NULL) != 0)
	{
		return EXIT_FAILURE;
	}
	if (pthread_create(&thread1, NULL, thread_func, NULL) != 0)
	{
		return EXIT_FAILURE;
	}
	for (i = 0; i < 20; i++)
	{
		puts("a");
		wait_thread();
	}

	if (pthread_join(thread, NULL) != 0)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
