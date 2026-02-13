
#include "philo.h"

void print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!*philo->stop_flag)
	{
		long time = get_elapsed_ms(&philo->rules->start_time);
		printf("%ld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}
