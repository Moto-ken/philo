
#include "philo.h"

// 人数, 餓死時間, 食事時間, 睡眠時間, 何回食べるか
t_philo *philosopher_create(t_rules *rules)
{
	t_philo *philos = malloc(sizeof(t_philo) * rules->number_of_philosophers);
	if (!philos)
		return (NULL);
	int i = 0;
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
		i++;
	}
	return (philos);
}

#include <string.h>
t_rules *rules_create(int argc, char **argv)
{
	t_rules *rules = malloc(sizeof(t_rules));
	if (!rules)
		return (NULL);
	// todo:swap ft_atoi
	rules->number_of_philosophers = atoi(argv[1]);
	rules->time_to_die = atol(argv[2]);
	rules->time_to_eat = atol(argv[3]);
	rules->time_to_sleep = atol(argv[4]);
	if (argc == 5)
		rules->number_of_times_each_philosopher_must_eat = 0;
	else if (argc == 6)
		rules->number_of_times_each_philosopher_must_eat = atoi(argv[5]);

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->number_of_philosophers);
	if (!rules->forks)
		return (1);
	int i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_init(&rules->forks[i], NULL);
		i++;
	}

	return (rules);
}

// 出力形式
// timestamp_in_ms X has taken a fork → フォークを取った
// timestamp_in_ms X is eating        → 食事している（食べるのにかかる時間）
// timestamp_in_ms X is sleeping      → 寝ている（寝るのにかかる時間）
// timestamp_in_ms X is thinking      → 考えている
// timestamp_in_ms X died             → 死亡した

int main(int argc, char **argv)
{
	if (argc != 6 && argc != 5)
		return (1);

	t_rules *rules = rules_create(argc, argv);
	if (!rules)
		return (1);
	t_philo *philos = philosopher_create(rules);
	if (!philos)
	{
		// todo:make func
		free_rules(rules);
		return (1);
	}

	// make thread

	free_rules(rules);
	free_philos(philos);
	return (0);
}
