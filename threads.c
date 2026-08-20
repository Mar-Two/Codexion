#include "header.h"

void	one_coder_in_table(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongle_mutex);
	coder->data->dongles[coder->left_dongle].taken_by = coder->id;
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	sleep_time(coder->data->time_to_burnout);
}

static int cooldown_blocking(t_coder *coder)
{
	if (timestamp(coder->data->start_time) <
	coder->data->dongles[coder->left_dongle].available_at)
		return 1;
	if (timestamp(coder->data->start_time) <
	coder->data->dongles[coder->right_dongle].available_at)
		return 1;
	return (0);
}

static int other_blocking(t_coder *coder)
{
	t_request out;
	if (coder->data->dongles[coder->left_dongle].taken_by != -1)
		return 1;
	if (coder->data->dongles[coder->right_dongle].taken_by != -1)
		return 1;
	consultation(&coder->data->dongles[coder->left_dongle].heap, &out);
	if (out.id != coder->id)
		return 1;
	consultation(&coder->data->dongles[coder->right_dongle].heap, &out);
	if(out.id != coder->id)
		return 1;
	return (0);
}

static int predicat(t_coder *coder)
{
	if (cooldown_blocking(coder) == 1)
		return 1;
	if(other_blocking(coder) == 1)
		return 1;
	return 0;
}

static long most_available_at(t_coder *coder)
{
	if (coder->data->dongles[coder->left_dongle].available_at >
		coder->data->dongles[coder->right_dongle].available_at )
		return coder->data->dongles[coder->left_dongle].available_at;
	return coder->data->dongles[coder->right_dongle].available_at;
}

int	try_take_dongles(t_coder *coder)
{
	t_request request;
	struct timespec	deadline;
	long ms;
	
	pthread_mutex_lock(&coder->coder_mutex);
	request.id = coder->id;
	if (coder->data->fifo == 1)
		request.key = timestamp(coder->data->start_time); 
	else
		request.key = coder->last_compile_start + coder->data->time_to_burnout; 
	pthread_mutex_unlock(&coder->coder_mutex);

	pthread_mutex_lock(&coder->data->dongle_mutex);
	insertion(&coder->data->dongles[coder->left_dongle].heap, &request);
	insertion(&coder->data->dongles[coder->right_dongle].heap, &request);
	while(predicat(coder) != 0 && coder->data->stop == 0)
	{
		if (cooldown_blocking(coder) == 1 && other_blocking(coder) == 0)
		{
			ms = most_available_at(coder);
			ms = coder->data->start_time + ms;
			deadline.tv_sec = ms / 1000;
			deadline.tv_nsec = (ms % 1000) * 1000000L;
			pthread_cond_timedwait(&coder->data->condvar,
				&coder->data->dongle_mutex, &deadline);
		}
		else
			pthread_cond_wait(&coder->data->condvar, &coder->data->dongle_mutex);
	}
	if (coder->data->stop == 1)
	{
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return 1;
	}
	extract_min(&coder->data->dongles[coder->left_dongle].heap, &request);
	extract_min(&coder->data->dongles[coder->right_dongle].heap, &request);
	coder->data->dongles[coder->left_dongle].taken_by = coder->id;
	coder->data->dongles[coder->right_dongle].taken_by = coder->id;
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	return 0;
}
