#include "philo.h"

void	free_philos(t_philo *philos, int n)
{
	int i;

	i = 0;
	if (!philos)
		return;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	free(philos);
}

void	free_rules(t_rules *rules)
{
	int i;

	if (!rules)
		return;
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
	int i;

	// 失敗することはそもそもあるかわからんけど、freeとかたりないかも
	if (pthread_join(monitor_thread, NULL) != 0)
		return;
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		if (pthread_join(philos[i].thread_id, NULL) != 0)
			return;
		i++;
	}

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&rules->print_mutex);
	free(philos);
	free(rules);
}
