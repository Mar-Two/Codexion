/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquisition.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:43:53 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 15:46:00 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	take_single_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongle_mutex);
	coder->data->dongles[coder->left_dongle].taken_by = coder->id;
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

static void	build_request(t_coder *coder, t_request *request)
{
	pthread_mutex_lock(&coder->coder_mutex);
	request->id = coder->id;
	request->done = coder->nb_compiles;
	if (coder->data->fifo == 1)
		request->key = timestamp_us(coder->data->start_time_us);
	else
		request->key = coder->last_compile_start
			+ (coder->data->time_to_burnout * 1000);
	pthread_mutex_unlock(&coder->coder_mutex);
}

static void	log_taken(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->print_mutex);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	printf("%ld %d has taken a dongle\n",
		timestamp(coder->data->start_time), coder->id);
	pthread_mutex_unlock(&coder->data->print_mutex);
}

static void	acquire_dongles(t_coder *coder)
{
	heap_remove(&coder->data->dongles[coder->left_dongle].heap, coder->id);
	heap_remove(&coder->data->dongles[coder->right_dongle].heap, coder->id);
	coder->data->dongles[coder->left_dongle].taken_by = coder->id;
	coder->data->dongles[coder->right_dongle].taken_by = coder->id;
	log_taken(coder);
	pthread_cond_broadcast(&coder->data->condvar);
}

int	try_take_dongles(t_coder *coder)
{
	t_request	request;

	build_request(coder, &request);
	pthread_mutex_lock(&coder->data->dongle_mutex);
	heap_push(&coder->data->dongles[coder->left_dongle].heap, &request);
	heap_push(&coder->data->dongles[coder->right_dongle].heap, &request);
	while (predicate(coder) != 0 && coder->data->stop == 0)
	{
		wait_for_dongles(coder);
	}
	if (coder->data->stop == 1)
	{
		heap_remove(&coder->data->dongles[coder->left_dongle].heap, coder->id);
		heap_remove(&coder->data->dongles[coder->right_dongle].heap, coder->id);
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return (1);
	}
	acquire_dongles(coder);
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	return (0);
}
