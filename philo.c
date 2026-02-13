
#include "philo.h"

// 人数, 餓死時間, 食事時間, 睡眠時間, 何回食べるか
t_philo *philosopher_init(t_rules *rules, bool *stop_flag)
{
	t_philo *philos;
	int i;

	philos = malloc(sizeof(t_philo) * rules->number_of_philosophers);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = 0;
		philos[i].meals_eaten = 0;
		philos[i].rules = rules;
		// もしかしたらNULLにしないほうがいいかも
		if (rules->number_of_philosophers == 1)
		{
			philos[0].left_fork = &rules->forks[0];
			philos[0].right_fork = NULL;
		}
		else
		{
			philos[i].left_fork = &rules->forks[i];
			philos[i].right_fork = &rules->forks[(i + 1) % rules->number_of_philosophers];
		}
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		philos[i].stop_flag = stop_flag;
		i++;
	}
	return (philos);
}

#include <string.h>
int atoi_datas(t_rules *rules, char **argv)
{
	rules->number_of_philosophers = atoi(argv[1]);
	if (rules->number_of_philosophers <= 0)
		return (1);

	rules->time_to_die = atol(argv[2]);
	if (rules->time_to_die <= 0)
		return (1);

	rules->time_to_eat = atol(argv[3]);
	if (rules->time_to_eat <= 0)
		return (1);

	rules->time_to_sleep = atol(argv[4]);
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
	// todo:swap ft_atoi
	// rules->number_of_philosophers = atoi(argv[1]);
	// if (rules->number_of_philosophers <= 0)
	// {
	// 	free(rules);
	// 	return (NULL);
	// }
	// rules->time_to_die = atol(argv[2]);
	// if (rulese->time_to_die <= 0)
	// {
	// 	free(rules);
	// 	return (NULL);
	// }
	// rules->time_to_eat = atol(argv[3]);
	// if (rules->time_to_eat <= 0)
	// {
	// 	free(rules);
	// 	return (NULL);
	// }
	// rules->time_to_sleep = atol(argv[4]);
	// if (rules->time_to_sleep <= 0)
	// {
	// 	free(rules);
	// 	return (NULL);
	// }
	if (atoi_datas(rules, argv))
	{
		free(rules);
		return (NULL);
	}
	gettimeofday(&rules->start_time, NULL);
	if (argc == 5)
		rules->number_of_times_each_philosopher_must_eat = 0;
	else if (argc == 6)
		rules->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->number_of_philosophers);
	if (!rules->forks)
	{
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

int main(int argc, char **argv)
{
	t_rules *rules;
	bool stop_flag;
	t_philo *philos;
	pthread_t monitor_thread;

	if (argc != 6 && argc != 5)
		return (1);
	rules = rules_create(argc, argv);
	if (!rules)
		return (1);
	stop_flag = false;
	philos = philosopher_init(rules, &stop_flag);
	if (!philos)
	{
		free_rules(rules);
		return (1);
	}
	if (run_threads(rules, philos))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	if (create_monitor(philos, rules, &stop_flag, &monitor_thread))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	join_and_free(rules, philos, monitor_thread);
	return (0);
}
