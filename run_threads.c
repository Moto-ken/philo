
#include "philo.h"

long	get_elapsed_ms(struct timeval *start)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (long)((now.tv_sec - start->tv_sec) * 1000
				+ (now.tv_usec - start->tv_usec) / 1000);
}

// todo: remake print func
void print_status(t_philo *philo, char *msg)
{
    pthread_mutex_lock(&philo->rules->print_mutex);
    printf("%ld %d %s\n", get_elapsed_ms(&philo->rules->start_time), philo->id, msg);
    pthread_mutex_unlock(&philo->rules->print_mutex);
}

void precise_sleep(t_philo *philo, long duration_ms)
{
    struct timeval start;
	struct timeval now;
	
    gettimeofday(&start, NULL);
    while (!*(philo->stop_flag))
    {
        gettimeofday(&now, NULL);
        long elapsed = (now.tv_sec - start.tv_sec) * 1000
                     + (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= duration_ms)
            break;
        usleep(500);
    }
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!*(philo->stop_flag))
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
		}
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal_time = get_elapsed_ms(&philo->rules->start_time);
		pthread_mutex_unlock(&philo->meal_mutex);

		print_status(philo, "is eating");
		precise_sleep(philo, philo->rules->time_to_eat);

		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		print_status(philo, "is sleeping");
		precise_sleep(philo, philo->rules->time_to_sleep);
		// thinkingのときにマイクロ秒の遅延を入れると、安定するかも
		print_status(philo, "is thinking");
	}
	return (NULL);
}

int	run_threads(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		if (pthread_create(&philos[i].thread_id, NULL, philo_routine,
				&philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
