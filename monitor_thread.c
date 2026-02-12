
#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_monitor_args	*moniter;
	t_philo			*philos;
	t_rules			*rules;
	int				i;
	long			last_meal;
	long			now;

	moniter = (t_monitor_args *)arg;
	philos = moniter->philos;
	rules = moniter->rules;
	while (!*(moniter->stop_flag))
	{
		i = 0;
		while (i < rules->number_of_philosophers)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			last_meal = philos[i].last_meal_time;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			now = get_timestamp_ms();
			if (now - last_meal > rules->time_to_die)
			{
				pthread_mutex_lock(&rules->print_mutex);
				printf("%ld %d died\n", now, philos[i].id);
				pthread_mutex_unlock(&rules->print_mutex);
				*(moniter->stop_flag) = true;
				return (NULL);
			}
			i++;
		}
		// これないとめっちゃ重くなるらしい
		usleep(1000);
	}
	return (NULL);
}

int	create_monitor(t_philo *philos, t_rules *rules, bool *stop_flag,
		pthread_t *monitor_thread)
{
	t_monitor_args	*args;

	args = malloc(sizeof(t_monitor_args));
	if (!args)
		return (1);
	args->philos = philos;
	args->rules = rules;
	args->stop_flag = stop_flag;
	if (pthread_create(&monitor_thread, NULL, monitor_routine, args) != 0)
	{
		free(args);
		return (1);
	}
	// free?
	free(args);
	return (0);
}
