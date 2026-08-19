#include "header.h"

void	one_coder_in_table(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongles[coder->left_dongle].dongle_mutex);
	coder->data->dongles[coder->left_dongle].taken_by = coder->id;
	pthread_mutex_unlock(
		&coder->data->dongles[coder->left_dongle].dongle_mutex);
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
	sleep_time(coder->data->time_to_burnout);
}

void	try_take_dongles(t_coder *coder, int *acquired)
{
	if (coder->left_dongle < coder->right_dongle)
	{
		pthread_mutex_lock(
			&coder->data->dongles[coder->left_dongle].dongle_mutex);
		pthread_mutex_lock(
			&coder->data->dongles[coder->right_dongle].dongle_mutex);
	}
	else
	{
		pthread_mutex_lock(
			&coder->data->dongles[coder->right_dongle].dongle_mutex);
		pthread_mutex_lock(
			&coder->data->dongles[coder->left_dongle].dongle_mutex);
	}
	if (coder->data->dongles[coder->left_dongle].taken_by == -1
		&& coder->data->dongles[coder->right_dongle].taken_by == -1)
	{
		coder->data->dongles[coder->left_dongle].taken_by = coder->id;
		coder->data->dongles[coder->right_dongle].taken_by = coder->id;
		*acquired = 1;
	}
	pthread_mutex_unlock(
		&coder->data->dongles[coder->left_dongle].dongle_mutex);
	pthread_mutex_unlock(
		&coder->data->dongles[coder->right_dongle].dongle_mutex);
}

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
