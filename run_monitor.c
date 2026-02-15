
#include "philo.h"

static bool is_stopped_monitor(t_monitor_args *monitor)
{
	bool stop;

	pthread_mutex_lock(&monitor->rules->print_mutex);
	stop = *(monitor->stop_flag);
	pthread_mutex_unlock(&monitor->rules->print_mutex);
	return (stop);
}

static void print_died(t_monitor_args *monitor, t_philo *philo)
{
	t_rules *rules = monitor->rules;

	pthread_mutex_lock(&rules->print_mutex);
	*(monitor->stop_flag) = true;
	printf("%ld %d died\n", get_elapsed_ms(&rules->start_time), philo->id);
	pthread_mutex_unlock(&rules->print_mutex);
}

static bool check_death(t_monitor_args *monitor)
{
	t_philo *philos;
	t_rules *rules;
	long last_meal;
	long now;
	int i;

	philos = monitor->philos;
	rules = monitor->rules;
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		last_meal = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		now = get_elapsed_ms(&rules->start_time);
		if (now - last_meal > rules->time_to_die)
		{
			print_died(monitor, &philos[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

static bool check_all_full(t_monitor_args *monitor)
{
	t_philo *philos;
	t_rules *rules;
	bool full;
	int i;

	rules = monitor->rules;
	philos = monitor->philos;
	full = true;
	i = 0;
	if (rules->number_of_times_each_philosopher_must_eat <= 0)
		return (false);
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
		*(monitor->stop_flag) = true;
		pthread_mutex_unlock(&rules->print_mutex);
	}
	return (full);
}

void *monitor_routine(void *arg)
{
	t_monitor_args *monitor;

	monitor = (t_monitor_args *)arg;
	while (!is_stopped_monitor(monitor))
	{
		if (check_death(monitor))
		{
			free(monitor);
			return (NULL);
		}
		if (check_all_full(monitor))
		{
			free(monitor);
			return (NULL);
		}
		usleep(1000);
	}
	free(monitor);
	return (NULL);
}

int run_monitor(t_philo *philos, t_rules *rules, bool *stop_flag,
				pthread_t *monitor_thread)
{
	t_monitor_args *args;

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
