/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_thread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 14:09:19 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 18:06:37 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	run_philos(t_rules *rules, t_philo *philos)
{
	int	i;
	int	c;

	c = 0;
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		if (pthread_create(&philos[i].thread_id, NULL, philo_routine,
				&philos[i]) != 0)
		{
			c = i;
			*philos[i].stop_flag = true;
			while (i >= 0)
			{
				pthread_join(philos[i].thread_id, NULL);
				i--;
			}
			return (c);
		}
		i++;
	}
	return (i);
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
