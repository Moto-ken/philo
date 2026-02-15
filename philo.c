
#include "philo.h"

t_philo *philosopher_create(t_rules *rules, bool *stop_flag)
{
	t_philo *philos;
	int i;

	philos = malloc(sizeof(t_philo) * rules->number_of_philosophers);
	if (!philos)
		return (NULL);
	memset(philos, 0, sizeof(t_philo) * rules->number_of_philosophers);
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = 0;
		philos[i].meals_eaten = get_elapsed_ms(&rules->start_time);
		philos[i].rules = rules;
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
		pthread_mutex_init(&philos[i].meal_count_mutex, NULL);
		philos[i].stop_flag = stop_flag;
		i++;
	}
	return (philos);
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
	philos = philosopher_create(rules, &stop_flag);
	if (!philos)
	{
		free_rules(rules);
		return (1);
	}
	if (run_philos(rules, philos))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	if (run_monitor(philos, rules, &stop_flag, &monitor_thread))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	join_and_free(rules, philos, monitor_thread);
	return (0);
}
