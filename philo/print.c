/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:30 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/21 00:22:07 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_status(t_philo *philo, char *msg)
{
	long	now_us;
	long	now_ms;
	long	last_meal;

	now_us = get_elapsed_us(&philo->rules->start_time);
	now_ms = now_us / 1000;
	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (*philo->stop_flag || (now_us - last_meal) > (philo->rules->time_to_die
			* 1000))
	{
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return (1);
	}
	if (printf("%ld %d %s\n", now_ms, philo->id, msg) < 0)
	{
		*(philo->stop_flag) = true;
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
	return (0);
}

void	print_died(t_monitor_args *monitor, t_philo *philo)
{
	t_rules	*rules;
	long	now_us;
	long	now_ms;

	rules = monitor->rules;
	now_us = get_elapsed_us(&rules->start_time);
	now_ms = now_us / 1000;
	pthread_mutex_lock(&rules->print_mutex);
	*(monitor->stop_flag) = true;
	printf("%ld %d died\n", now_ms, philo->id);
	pthread_mutex_unlock(&rules->print_mutex);
}
