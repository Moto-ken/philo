/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:30 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 13:41:31 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	long	time;

	time = get_elapsed_ms(&philo->rules->start_time);
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!*philo->stop_flag)
		printf("%ld %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

void	print_died(t_monitor_args *monitor, t_philo *philo)
{
	t_rules	*rules;

	rules = monitor->rules;
	pthread_mutex_lock(&rules->print_mutex);
	*(monitor->stop_flag) = true;
	printf("%ld %d died\n", get_elapsed_ms(&rules->start_time), philo->id);
	pthread_mutex_unlock(&rules->print_mutex);
}
