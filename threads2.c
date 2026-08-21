#include "header.h"

void	coder_is_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile_start = timestamp(coder->data->start_time);
	coder->nb_compiles += 1;
	pthread_mutex_unlock(&coder->coder_mutex);
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	printf("%ld %d is compiling\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	sleep_time(coder->data->time_to_compile);
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

void	coder_is_debugging(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d is debugging\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	sleep_time(coder->data->time_to_debug);
}

void	coder_is_refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d is refactoring\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	sleep_time(coder->data->time_to_refactor);
}

int should_stop(t_coder	*coder)
{
	int result;

	result = 0;
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
		result = 1;
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	return result;
}

static void run_lonely_coder(t_coder *coder)
{
	one_coder_in_table(coder);
	pthread_mutex_lock(&coder->data->dongle_mutex);
	while(coder->data->stop == 0)
	{
		pthread_cond_wait(&coder->data->condvar, &coder->data->dongle_mutex);
	}
	pthread_mutex_unlock(&coder->data->dongle_mutex);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder*) arg;
	if (coder->left_dongle == coder->right_dongle)
	{
		run_lonely_coder(coder);
		return (NULL);
	}
	while (should_stop(coder) == 0)
	{
		if (try_take_dongles(coder) == 1)
			break;
		coder_is_compiling(coder);
		drop_dongles(coder);
		if(should_stop(coder) == 1)
			break;
		coder_is_debugging(coder);
		if(should_stop(coder) == 1)
			break;
		coder_is_refactoring(coder);
	}
	return (NULL);
}
