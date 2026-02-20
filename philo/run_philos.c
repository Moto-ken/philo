/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_philos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:38 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 19:43:03 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_two_forks(t_philo *philo, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (is_stopped_philo(philo))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second);
	if (is_stopped_philo(philo))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

static int	take_forks(t_philo *philo)
{
	if (is_stopped_philo(philo))
		return (1);
	if (philo->id % 2 == 0)
		return (take_two_forks(philo, philo->right_fork, philo->left_fork));
	else
		return (take_two_forks(philo, philo->left_fork, philo->right_fork));
}

static int	eat_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_elapsed_us(&philo->rules->start_time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (is_stopped_philo(philo))
		return (1);
	print_status(philo, "is eating");
	precise_sleep(philo, philo->rules->time_to_eat);
	pthread_mutex_lock(&philo->meal_count_mutex);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->meal_count_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

static bool	single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_elapsed_us(&philo->rules->start_time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (!philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		usleep(philo->rules->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		return (false);
	}
	if (philo->id % 2 == 0)
		usleep(philo->rules->time_to_eat * 500);
	return (true);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!single_philo(philo))
		return (NULL);
	while (!is_stopped_philo(philo))
	{
		print_status(philo, "is thinking");
		if (philo->id % 2 != 0)
			usleep(100);
		if (take_forks(philo))
			break ;
		if (eat_philo(philo))
			break ;
		usleep(100);
		print_status(philo, "is sleeping");
		precise_sleep(philo, philo->rules->time_to_sleep);
	}
	return (NULL);
}
