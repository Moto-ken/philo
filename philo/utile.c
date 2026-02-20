/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:40 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 20:37:18 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_elapsed_us(struct timeval *start)
{
	struct timeval	now;
	long			sec_diff;
	long			usec_diff;

	gettimeofday(&now, NULL);
	sec_diff = now.tv_sec - start->tv_sec;
	usec_diff = now.tv_usec - start->tv_usec;
	return (sec_diff * 1000000 + usec_diff);
}

void	precise_sleep(t_philo *philo, long duration_ms)
{
	struct timeval	start;
	long			elapsed_us;
	long			duration_us;

	duration_us = duration_ms * 1000;
	gettimeofday(&start, NULL);
	while (1)
	{
		pthread_mutex_lock(&philo->rules->print_mutex);
		if (*(philo->stop_flag))
		{
			pthread_mutex_unlock(&philo->rules->print_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->print_mutex);
		elapsed_us = get_elapsed_us(&start);
		if (elapsed_us >= duration_us)
			break ;
		usleep(500);
	}
}

int	atoi_datas(t_rules *rules, char **argv)
{
	if (!isnum(argv[1]))
		return (1);
	rules->number_of_philosophers = ft_atoi(argv[1]);
	if (rules->number_of_philosophers <= 0)
		return (1);
	if (!isnum(argv[2]))
		return (1);
	rules->time_to_die = ft_atol(argv[2]);
	if (rules->time_to_die <= 0)
		return (1);
	if (!isnum(argv[3]))
		return (1);
	rules->time_to_eat = ft_atol(argv[3]);
	if (rules->time_to_eat <= 0)
		return (1);
	if (!isnum(argv[4]))
		return (1);
	rules->time_to_sleep = ft_atol(argv[4]);
	if (rules->time_to_sleep <= 0)
		return (1);
	return (0);
}

bool	is_stopped_philo(t_philo *philo)
{
	bool	stop;

	pthread_mutex_lock(&philo->rules->print_mutex);
	stop = *(philo->stop_flag);
	pthread_mutex_unlock(&philo->rules->print_mutex);
	return (stop);
}
