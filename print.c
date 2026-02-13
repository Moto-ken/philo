
#include "philo.h"

// 出力形式
// timestamp_in_ms X has taken a fork → フォークを取った
// timestamp_in_ms X is eating        → 食事している（食べるのにかかる時間）
// timestamp_in_ms X is sleeping      → 寝ている（寝るのにかかる時間）
// timestamp_in_ms X is thinking      → 考えている
// timestamp_in_ms X died             → 死亡した

void print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!*philo->stop_flag)
	{
		long time = get_elapsed_ms(&philo->rules->start_time);
		printf("%ld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}
