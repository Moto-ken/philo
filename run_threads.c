
#include "philo.h"

// todo: remake print func
void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex);
	printf("%ld %d %s\n", get_timestamp(), philo->id, msg);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	precise_sleep(t_philo *philo, long duration_ms)
{
	long	start;

	start = get_timestamp_ms();
	while (!*(philo->stop_flag))
	{
		if (get_timestamp_ms() - start >= duration_ms)
			break;
		usleep(100);
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
		philo->last_meal_time = get_timestamp_ms();
		pthread_mutex_unlock(&philo->meal_mutex);

		print_status(philo, "is eating");
		// usleep(philo->rules->time_to_eat * 1000);
		precise_sleep(philo, philo->rules->time_to_eat);

		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		print_status(philo, "is sleeping");
		// usleep(philo->rules->time_to_sleep * 1000);
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
