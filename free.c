
#include "philo.h"

void free_philos(t_philo *philos, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	free(philos);
}

void free_rules(t_rules *rules)
{
	int i;

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	free(rules->forks);
	free(rules);
}
