#include "header.h"

int	coder_is_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile_start = timestamp(coder->data->start_time);
	coder->nb_compiles += 1;
	pthread_mutex_unlock(&coder->coder_mutex);
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
	{
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return (1);
	}
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	printf("%ld %d is compiling\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	sleep_time(coder->data->time_to_compile);
	return (0);
}

void	drop_dongles(t_coder *coder)
{
	int		left;
	int		right;
	long	cooldown;
	long	start_time;

	left = coder->left_dongle;
	right = coder->right_dongle;
	cooldown = coder->data->dongle_cooldown;
	start_time = coder->data->start_time;
	pthread_mutex_lock(&coder->data->dongle_mutex);
	coder->data->dongles[left].available_at = timestamp(start_time) + cooldown;
	coder->data->dongles[right].available_at = timestamp(start_time) + cooldown;
	coder->data->dongles[left].taken_by = -1;
	coder->data->dongles[right].taken_by = -1;
	pthread_cond_broadcast(&coder->data->condvar);
	pthread_mutex_unlock(&coder->data->dongle_mutex);
}

int	coder_is_debugging(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
	{
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return (1);
	}
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d is debugging\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	sleep_time(coder->data->time_to_debug);
	return (0);
}

int	coder_is_refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
	{
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return (1);
	}
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d is refactoring\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	sleep_time(coder->data->time_to_refactor);
	return (0);
}