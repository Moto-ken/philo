/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_create.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:23 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 13:41:28 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	decide_forks(t_philo *philo, t_rules *rules, int i)
{
	if (rules->number_of_philosophers == 1)
	{
		philo[0].left_fork = &rules->forks[0];
		philo[0].right_fork = NULL;
	}
	else
	{
		philo[i].left_fork = &rules->forks[i];
		philo[i].right_fork = &rules->forks[(i + 1)
			% rules->number_of_philosophers];
	}
}

static int	init_philo_mutexes(t_philo *philos, int i)
{
	if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
	{
		free_philos(philos, i);
		return (1);
	}
	if (pthread_mutex_init(&philos[i].meal_count_mutex, NULL) != 0)
	{
		free_philos(philos, i + 1);
		return (1);
	}
	return (0);
}

t_philo	*philosopher_create(t_rules *rules, bool *stop_flag)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * rules->number_of_philosophers);
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo) * rules->number_of_philosophers);
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = 0;
		philos[i].meals_eaten = get_elapsed_ms(&rules->start_time);
		philos[i].rules = rules;
		decide_forks(philos, rules, i);
		if (init_philo_mutexes(philos, i))
			return (NULL);
		philos[i].stop_flag = stop_flag;
		i++;
	}
	return (philos);
}
