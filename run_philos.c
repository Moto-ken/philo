/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_philos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:38 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/16 14:19:52 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_stopped_philo(t_philo *philo)
{
	bool	stop;

	pthread_mutex_lock(&philo->rules->print_mutex);
	stop = *(philo->stop_flag);
	pthread_mutex_unlock(&philo->rules->print_mutex);
	return (stop);
}

static void	single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	usleep(philo->rules->time_to_die * 1000);
	pthread_mutex_unlock(philo->left_fork);
}

static void	take_forks(t_philo *philo)
{
	if (is_stopped_philo(philo))
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		if (is_stopped_philo(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		if (is_stopped_philo(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return;
		}
		print_status(philo, "has taken a fork");
	}
    else
    {
        pthread_mutex_lock(philo->left_fork);
        if (is_stopped_philo(philo))
        {
            pthread_mutex_unlock(philo->left_fork);
            return;
        }
        print_status(philo, "has taken a fork");

        pthread_mutex_lock(philo->right_fork);
        if (is_stopped_philo(philo))
        {
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            return;
        }
        print_status(philo, "has taken a fork");
    }
}

static void	eat_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_elapsed_ms(&philo->rules->start_time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (is_stopped_philo(philo))
		return ;
	print_status(philo, "is eating");
	precise_sleep(philo, philo->rules->time_to_eat);
	pthread_mutex_lock(&philo->meal_count_mutex);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->meal_count_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_elapsed_ms(&philo->rules->start_time);
	pthread_mutex_unlock(&philo->meal_mutex);
	if (!philo->right_fork)
	{
		single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(philo->rules->time_to_eat * 500);
	while (!is_stopped_philo(philo))
	{
		print_status(philo, "is thinking");
		if (philo->id % 2 != 0)
			usleep(100);
		take_forks(philo);
		eat_philo(philo);
		usleep(100);
		print_status(philo, "is sleeping");
		precise_sleep(philo, philo->rules->time_to_sleep);
	}
	return (NULL);
}
