
#include "philo.h"

static int atoi_datas(t_rules *rules, char **argv)
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

static int set_meal_limit(t_rules *rules, int argc, char **argv)
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
		rules->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
		return (0);
	}
	return (1);
}

static void cleanup_rules(t_rules *rules)
{
	if (!rules)
		return;
	if (rules->forks)
		free(rules->forks);
	pthread_mutex_destroy(&rules->print_mutex);
	free(rules);
}

static int init_forks_mutexes(t_rules *rules)
{
	int i;
	int j;

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

static int init_rules_resources(t_rules *rules, int argc, char **argv)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
		return 1;
	if (set_meal_limit(rules, argc, argv))
		return 1;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->number_of_philosophers);
	if (!rules->forks)
		return 1;
	if (init_forks_mutexes(rules))
		return 1;
	return 0;
}

t_rules *rules_create(int argc, char **argv)
{
	t_rules *rules;

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
	{
		cleanup_rules(rules);
		return (NULL);
	}
	return (rules);
}
