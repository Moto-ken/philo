/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:32 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 17:23:58 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	set_meal_limit(t_rules *rules, int argc, char **argv)
{
	if (argc == 5)
	{
		rules->number_of_times_each_philosopher_must_eat = 0;
		return (0);
	}
	else if (argc == 6)
	{
		if (!isnum(argv[5]))
			return (1);
		rules->number_of_times_each_philosopher_must_eat = ft_atol(argv[5]);
		if (rules->number_of_times_each_philosopher_must_eat <= 0)
			return (1);
		return (0);
	}
	return (1);
}

static int	init_forks_mutexes(t_rules *rules)
{
	int	i;
	int	j;

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
		{
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(&rules->forks[j]);
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_rules_resources(t_rules *rules, int argc, char **argv)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		free(rules);
		return (1);
	}
	if (set_meal_limit(rules, argc, argv))
	{
		cleanup_rules(rules);
		return (1);
	}
	rules->forks = malloc(sizeof(pthread_mutex_t)
			* rules->number_of_philosophers);
	if (!rules->forks)
	{
		cleanup_rules(rules);
		return (1);
	}
	if (init_forks_mutexes(rules))
	{
		cleanup_rules(rules);
		return (1);
	}
	return (0);
}

t_rules	*rules_create(int argc, char **argv)
{
	t_rules	*rules;

	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (NULL);
	memset(rules, 0, sizeof(t_rules));
	if (atoi_datas(rules, argv))
	{
		free(rules);
		return (NULL);
	}
	gettimeofday(&rules->start_time, NULL);
	if (init_rules_resources(rules, argc, argv))
		return (NULL);
	return (rules);
}
