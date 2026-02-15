
#include "philo.h"

long get_elapsed_ms(struct timeval *start)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (long)((now.tv_sec - start->tv_sec) * 1000 + (now.tv_usec - start->tv_usec) / 1000);
}

void precise_sleep(t_philo *philo, long duration_ms)
{
	struct timeval start;
	long elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->rules->print_mutex);
		if (*(philo->stop_flag))
		{
			pthread_mutex_unlock(&philo->rules->print_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->rules->print_mutex);

		elapsed = get_elapsed_ms(&start);
		if (elapsed >= duration_ms)
			break;

		usleep(500);
	}
}
