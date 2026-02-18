/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:03 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 13:41:05 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philos(t_philo *philos, int n)
{
	int	i;

	i = 0;
	if (!philos)
		return ;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		pthread_mutex_destroy(&philos[i].meal_count_mutex);
		i++;
	}
	free(philos);
}

void	cleanup_rules(t_rules *rules)
{
	if (!rules)
		return ;
	if (rules->forks)
		free(rules->forks);
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules);
}

void	free_rules(t_rules *rules)
{
	int	i;

	if (!rules)
		return ;
	if (rules->forks)
	{
		i = 0;
		while (i < rules->number_of_philosophers)
		{
			pthread_mutex_destroy(&rules->forks[i]);
			i++;
		}
		free(rules->forks);
	}
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules);
}

void	join_and_free(t_rules *rules, t_philo *philos, pthread_t monitor_thread)
{
	int	i;

	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	free_philos(philos, rules->number_of_philosophers);
	free_rules(rules);
}
