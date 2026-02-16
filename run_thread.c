/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_thread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:09:19 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 14:10:13 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	run_philos(t_rules *rules, t_philo *philos)
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
