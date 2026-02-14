
#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_monitor_args	*moniter;
	t_philo			*philos;
	t_rules			*rules;
	int				i;
	bool			full;
	long			last_meal;
	long			now;

	moniter = (t_monitor_args *)arg;
	philos = moniter->philos;
	rules = moniter->rules;
	while (1)
	{
		pthread_mutex_lock(&rules->print_mutex);
		if (*(moniter->stop_flag))
		{
			pthread_mutex_unlock(&rules->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&rules->print_mutex);

		i = 0;
		while (i < rules->number_of_philosophers)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			last_meal = philos[i].last_meal_time;
			pthread_mutex_unlock(&philos[i].meal_mutex);
			now = get_elapsed_ms(&rules->start_time);
			if (now - last_meal > rules->time_to_die)
			{
				print_status(&philos[i], "died");
				pthread_mutex_lock(&rules->print_mutex);
				*(moniter->stop_flag) = true;
				pthread_mutex_unlock(&rules->print_mutex);
				return (NULL);
			}
			i++;
		}
		if (rules->number_of_times_each_philosopher_must_eat > 0)
		{
			full = true;
			i = 0;
			while (i < rules->number_of_philosophers)
			{
				pthread_mutex_lock(&philos[i].meal_count_mutex);
				if (philos[i].meal_count < rules->number_of_times_each_philosopher_must_eat)
					full = false;
				pthread_mutex_unlock(&philos[i].meal_count_mutex);
				i++;
			}
			if (full)
			{
				pthread_mutex_lock(&rules->print_mutex);
				*(moniter->stop_flag) = true;
				pthread_mutex_unlock(&rules->print_mutex);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	run_monitor(t_philo *philos, t_rules *rules, bool *stop_flag,
		pthread_t *monitor_thread)
{
	t_monitor_args	*args;

	args = malloc(sizeof(t_monitor_args));
	if (!args)
		return (1);
	args->philos = philos;
	args->rules = rules;
	args->stop_flag = stop_flag;
	if (pthread_create(monitor_thread, NULL, monitor_routine, args) != 0)
	{
		free(args);
		return (1);
	}
	return (0);
}
