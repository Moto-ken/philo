/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:30 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 20:23:11 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	print_status(t_philo *philo, char *msg)
// {
// 	long	now;

// 	now = get_elapsed_ms(&philo->rules->start_time);
// 	pthread_mutex_lock(&philo->rules->print_mutex);
// 	if (*philo->stop_flag || now
// 		- philo->last_meal_time > philo->rules->time_to_die)
// 	{
// 		pthread_mutex_unlock(&philo->rules->print_mutex);
// 		return ;
// 	}
// 	printf("%ld %d %s\n", now, philo->id, msg);
// 	pthread_mutex_unlock(&philo->rules->print_mutex);
// }

void	print_status(t_philo *philo, char *msg)
{
	long	now_us;
	long	now_ms;

	now_us = get_elapsed_us(&philo->rules->start_time);
	now_ms = now_us / 1000;
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (*philo->stop_flag || (now_us
			- philo->last_meal_time) > (philo->rules->time_to_die * 1000))
	{
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return ;
	}
	printf("%ld %d %s\n", now_ms, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

// void	print_died(t_monitor_args *monitor, t_philo *philo)
// {
// 	t_rules	*rules;

// 	rules = monitor->rules;
// 	pthread_mutex_lock(&rules->print_mutex);
// 	*(monitor->stop_flag) = true;
// 	printf("%ld %d died\n", get_elapsed_ms(&rules->start_time), philo->id);
// 	pthread_mutex_unlock(&rules->print_mutex);
// }

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
