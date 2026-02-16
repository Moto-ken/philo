
#include "philo.h"

static int init_all(int argc, char **argv, t_rules **rules, t_philo **philos, bool *stop_flag)
{
	*rules = rules_create(argc, argv);
	if (!*rules)
		return (1);

	*stop_flag = false;

	*philos = philosopher_create(*rules, stop_flag);
	if (!*philos)
	{
		free_rules(*rules);
		return (1);
	}
	return (0);
}

static int run_all(t_rules *rules, t_philo *philos, bool *stop_flag, pthread_t *monitor_thread)
{
	if (run_philos(rules, philos))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}

	if (run_monitor(philos, rules, stop_flag, monitor_thread))
	{
		free_philos(philos, rules->number_of_philosophers);
		free_rules(rules);
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_rules *rules;
	t_philo *philos;
	bool stop_flag;
	pthread_t monitor_thread;

	if (argc != 5 && argc != 6)
		return (1);

	if (init_all(argc, argv, &rules, &philos, &stop_flag))
		return (1);

	if (run_all(rules, philos, &stop_flag, &monitor_thread))
		return (1);

	join_and_free(rules, philos, monitor_thread);
	return (0);
}
