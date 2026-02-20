/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:35 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 19:43:51 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_stopped_monitor(t_monitor_args *monitor)
{
	bool	stop;

	pthread_mutex_lock(&monitor->rules->print_mutex);
	stop = *(monitor->stop_flag);
	pthread_mutex_unlock(&monitor->rules->print_mutex);
	return (stop);
}

static bool	check_death(t_monitor_args *monitor)
{
	t_philo	*philos;
	t_rules	*rules;
	long	last_meal;
	long	now;
	int		i;

	philos = monitor->philos;
	rules = monitor->rules;
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		last_meal = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		now = get_elapsed_us(&rules->start_time);
		if ((now - last_meal) / 1000 > rules->time_to_die)
		{
			print_died(monitor, &philos[i]);
			return (true);
		}
		i++;
	}
	return (false);
}

static bool	chenge_full(t_philo *philos, t_rules *rules)
{
	bool	full;
	int		i;

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
	return (full);
}

static bool	check_all_full(t_monitor_args *monitor)
{
	t_philo	*philos;
	t_rules	*rules;
	bool	full;

	rules = monitor->rules;
	philos = monitor->philos;
	full = true;
	if (rules->number_of_times_each_philosopher_must_eat <= 0)
		return (false);
	full = chenge_full(philos, rules);
	if (full)
	{
		pthread_mutex_lock(&rules->print_mutex);
		*(monitor->stop_flag) = true;
		pthread_mutex_unlock(&rules->print_mutex);
	}
	return (full);
}

void	*monitor_routine(void *arg)
{
	t_monitor_args	*monitor;

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
