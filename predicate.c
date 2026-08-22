#include "header.h"

static int	cooldown_blocking(t_coder *coder)
{
	if (timestamp(coder->data->start_time) <
	coder->data->dongles[coder->left_dongle].available_at)
		return 1;
	if (timestamp(coder->data->start_time) <
	coder->data->dongles[coder->right_dongle].available_at)
		return 1;
	return (0);
}

static int	other_blocking(t_coder *coder)
{
	t_request	out;

	if (coder->data->dongles[coder->left_dongle].taken_by != -1)
		return (1);
	if (coder->data->dongles[coder->right_dongle].taken_by != -1)
		return (1);
	heap_peek(&coder->data->dongles[coder->left_dongle].heap, &out);
	if (out.id != coder->id)
		return (1);
	heap_peek(&coder->data->dongles[coder->right_dongle].heap, &out);
	if(out.id != coder->id)
		return (1);
	return (0);
}

int	predicate(t_coder *coder)
{
	if (cooldown_blocking(coder) == 1)
		return (1);
	if(other_blocking(coder) == 1)
		return (1);
	return (0);
}

static long	most_available_at(t_coder *coder)
{
	if (coder->data->dongles[coder->left_dongle].available_at >
		coder->data->dongles[coder->right_dongle].available_at )
		return coder->data->dongles[coder->left_dongle].available_at;
	return coder->data->dongles[coder->right_dongle].available_at;
}


void	wait_for_dongles(t_coder *coder)
{
	struct timespec	deadline;
	long				ms;

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