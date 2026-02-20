/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:41:18 by kemotoha          #+#    #+#             */
/*   Updated: 2026/02/20 20:37:10 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*forks;
	struct timeval	start_time;
	pthread_mutex_t	print_mutex;
}					t_rules;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	pthread_t		thread_id;
	t_rules			*rules;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	bool			*stop_flag;
	int				meal_count;
	pthread_mutex_t	meal_count_mutex;
}					t_philo;

typedef struct s_monitor_args
{
	t_philo			*philos;
	t_rules			*rules;
	bool			*stop_flag;
}					t_monitor_args;

long				get_elapsed_us(struct timeval *start);
void				precise_sleep(t_philo *philo, long duration_ms);
int					atoi_datas(t_rules *rules, char **argv);
int					isnum(char *str);
int					ft_atoi(const char *str);
long				ft_atol(const char *str);
bool				is_stopped_philo(t_philo *philo);

void				print_status(t_philo *philo, char *msg);
void				print_died(t_monitor_args *monitor, t_philo *philo);

t_philo				*philosopher_create(t_rules *rules, bool *stop_flag);
t_rules				*rules_create(int argc, char **argv);

int					run_philos(t_rules *rules, t_philo *philos);
int					run_monitor(t_philo *philos, t_rules *rules,
						bool *stop_flag, pthread_t *monitor_thread);

void				*philo_routine(void *arg);
void				*monitor_routine(void *arg);

void				free_philos(t_philo *philos, int n);
void				free_rules(t_rules *rules);
void				cleanup_rules(t_rules *rules);
void				join_and_free(t_rules *rules, t_philo *philos,
						pthread_t monitor_thread);

#endif
