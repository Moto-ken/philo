/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:12 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 18:15:16 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	rules_init(int argc, char **argv, t_rules **rules, bool *stop_flag)
{
	*rules = rules_create(argc, argv);
	if (!*rules)
		return (1);
	*stop_flag = false;
	return (0);
}

static int	philosopher_init(t_rules *rules, t_philo **philos, bool *stop_flag)
{
	*philos = philosopher_create(rules, stop_flag);
	if (!*philos)
	{
		free_rules(rules);
		return (1);
	}
	return (0);
}

static int	run_all(t_rules *rules, t_philo *philos, bool *stop_flag,
		pthread_t *monitor_thread)
{
	int	succes;

	succes = run_philos(rules, philos);
	if (succes < rules->number_of_philosophers)
	{
		free_philos(philos, succes);
		free_rules(rules);
		return (1);
	}
	if (run_monitor(philos, rules, stop_flag, monitor_thread))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules		*rules;
	t_philo		*philos;
	bool		stop_flag;
	pthread_t	monitor_thread;

	if (argc != 5 && argc != 6)
		return (1);
	if (rules_init(argc, argv, &rules, &stop_flag))
		return (1);
	if (philosopher_init(rules, &philos, &stop_flag))
		return (1);
	if (run_all(rules, philos, &stop_flag, &monitor_thread))
		return (1);
	join_and_free(rules, philos, monitor_thread);
	return (0);
}
