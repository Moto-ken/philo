
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

// 引数の意味（日本語）
// number_of_philosophers → 哲学者の数
// time_to_die → 哲学者が最後に食べてから死亡するまでの時間（ミリ秒）
// time_to_eat → 食事にかかる時間（ミリ秒）
// time_to_sleep → 睡眠にかかる時間（ミリ秒）
// [number_of_times_each_philosopher_must_eat]（オプション）→ 各哲学者が食事する回数（指定がなければ無制限）
typedef struct s_rules
{
	int				number_of_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*forks;
	struct timeval	start_time;
	pthread_mutex_t	print_mutex;
}					t_rules;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	pthread_t		thread_id;
	t_rules			*rules;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	bool			*stop_flag;
}					t_philo;

// mutex stop_flag
typedef struct s_monitor_args
{
	t_philo			*philos;
	t_rules			*rules;
	bool			*stop_flag;
}					t_monitor_args;

// --- ユーティリティ関数 ---
long        get_elapsed_ms(struct timeval *start);
void        precise_sleep(t_philo *philo, long duration_ms);
int         isnum(char *str);
int         ft_atoi(const char *str);
long        ft_atol(const char *str);

// --- 出力関係 ---
void        print_status(t_philo *philo, char *msg);

// --- 初期化・作成 ---
t_philo     *philosopher_init(t_rules *rules, bool *stop_flag);
t_rules     *rules_create(int argc, char **argv);
int         atoi_datas(t_rules *rules, char **argv);

// --- スレッド関連 ---
int         run_philos(t_rules *rules, t_philo *philos);
int         run_monitor(t_philo *philos, t_rules *rules, bool *stop_flag,
                        pthread_t *monitor_thread);

// --- 後片付け ---
void        free_philos(t_philo *philos, int n);
void        free_rules(t_rules *rules);
void        join_and_free(t_rules *rules, t_philo *philos, pthread_t monitor_thread);

#endif
