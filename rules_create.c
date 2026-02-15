
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

t_rules *rules_create(int argc, char **argv)
{
	t_rules *rules;
	int i;

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
	pthread_mutex_init(&rules->print_mutex, NULL);
	if (argc == 5)
		rules->number_of_times_each_philosopher_must_eat = 0;
	else if (argc == 6)
	{
		if (!isnum(argv[5]))
		{
			free(rules);
			return (NULL);
		}
		rules->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	}
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->number_of_philosophers);
	if (!rules->forks)
	{
		pthread_mutex_destroy(&rules->print_mutex);
		free(rules);
		return (NULL);
	}
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_init(&rules->forks[i], NULL);
		i++;
	}
	return (rules);
}
