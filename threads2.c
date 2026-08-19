#include "header.h"

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
	if (left < right)
	{
		pthread_mutex_lock(&coder->data->dongles[left].dongle_mutex);
		pthread_mutex_lock(&coder->data->dongles[right].dongle_mutex);
	}
	else
	{
		pthread_mutex_lock(&coder->data->dongles[right].dongle_mutex);
		pthread_mutex_lock(&coder->data->dongles[left].dongle_mutex);
	}
	coder->data->dongles[left].available_at = timestamp(start_time) + cooldown;
	coder->data->dongles[right].available_at = timestamp(start_time) + cooldown;
	coder->data->dongles[left].taken_by = -1;
	coder->data->dongles[right].taken_by = -1;
	pthread_mutex_unlock(&coder->data->dongles[left].dongle_mutex);
	pthread_mutex_unlock(&coder->data->dongles[right].dongle_mutex);
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

void	*routine(void *arg)
{
	t_coder	*coder;
	int		acquired;
	int		tour;

	tour = 1;
	coder = (t_coder*) arg;
	while (tour == 1)
	{
		acquired = 0;
		if (coder->left_dongle == coder->right_dongle)
		{
			one_coder_in_table(coder);
			break;
		}
		else
			try_take_dongles(coder, &acquired);
		if (acquired == 1)
		{
			coder_is_compiling(coder);
			drop_dongles(coder);
			coder_is_debugging(coder);
			coder_is_refactoring(coder);
		}
		usleep(500);
	}
	return (NULL);
}
