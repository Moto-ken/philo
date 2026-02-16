/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:40 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 13:53:37 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// long	get_elapsed_ms(struct timeval *start)
// {
// 	struct timeval	now;

// 	gettimeofday(&now, NULL);
// 	return (long)((now.tv_sec - start->tv_sec) * 1000 + (now.tv_usec
// 			- start->tv_usec) / 1000);
// }

long	get_elapsed_ms(struct timeval *start)
{
	struct timeval	now;
	long			sec_diff;
	long			usec_diff;
	long			elapsed_ms;

	gettimeofday(&now, NULL);
	sec_diff = now.tv_sec - start->tv_sec;
	usec_diff = now.tv_usec - start->tv_usec;
	elapsed_ms = sec_diff * 1000 + usec_diff / 1000;
	return (elapsed_ms);
}

void	precise_sleep(t_philo *philo, long duration_ms)
{
	struct timeval	start;
	long			elapsed;

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
		elapsed = get_elapsed_ms(&start);
		if (elapsed >= duration_ms)
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
