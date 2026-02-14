
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
	long now;

	gettimeofday(&start, NULL);
	while (!*(philo->stop_flag))
	{
		if ((now = get_elapsed_ms(&start)) >= duration_ms)
			break;
		usleep(500);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->rules->print_mutex);
		if (*(philo->stop_flag))
		{
			pthread_mutex_unlock(&philo->rules->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->print_mutex);

		if (philo->id % 2 == 0)
		{
			if (philo->right_fork)
				pthread_mutex_lock(philo->right_fork);
			pthread_mutex_lock(philo->left_fork);
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			if (philo->right_fork)
				pthread_mutex_lock(philo->right_fork);
		}
		if (philo->right_fork)
		{
			pthread_mutex_lock(&philo->meal_mutex);
			philo->last_meal_time = get_elapsed_ms(&philo->rules->start_time);
			pthread_mutex_unlock(&philo->meal_mutex);

			pthread_mutex_lock(&philo->meal_count_mutex);
			philo->meal_count++;
			pthread_mutex_unlock(&philo->meal_count_mutex);

			print_status(philo, "is eating");
			precise_sleep(philo, philo->rules->time_to_eat);
		}

		pthread_mutex_unlock(philo->left_fork);
		if (philo->right_fork)
			pthread_mutex_unlock(philo->right_fork);

		print_status(philo, "is sleeping");
		precise_sleep(philo, philo->rules->time_to_sleep);

		usleep(1000);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

int run_philos(t_rules *rules, t_philo *philos)
{
	int i;

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
