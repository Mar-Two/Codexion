/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 15:47:36 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 15:47:39 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	coder_is_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
	{
		pthread_mutex_unlock(&coder->data->dongle_mutex);
		return (1);
	}
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile_start = timestamp_us(coder->data->start_time_us);
	coder->nb_compiles += 1;
	pthread_mutex_unlock(&coder->coder_mutex);
	pthread_mutex_lock(&coder->data->print_mutex);
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

	left = coder->left_dongle;
	right = coder->right_dongle;
	pthread_mutex_lock(&coder->data->dongle_mutex);
	cooldown = ((coder->last_compile_start / 1000)
			+ coder->data->time_to_compile + coder->data->dongle_cooldown)
		* 1000;
	coder->data->dongles[left].available_at = cooldown;
	coder->data->dongles[right].available_at = cooldown;
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
