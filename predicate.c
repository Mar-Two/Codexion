/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   predicate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:13:21 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 18:00:00 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	cooldown_blocking(t_coder *coder)
{
	if (timestamp_us(coder->data->start_time_us)
		< coder->data->dongles[coder->left_dongle].available_at)
		return (1);
	if (timestamp_us(coder->data->start_time_us)
		< coder->data->dongles[coder->right_dongle].available_at)
		return (1);
	return (0);
}

static int	other_blocking(t_coder *coder)
{
	if (coder->data->dongles[coder->left_dongle].taken_by != -1)
		return (1);
	if (coder->data->dongles[coder->right_dongle].taken_by != -1)
		return (1);
	if (grants_to_me(coder, coder->left_dongle) == 0)
		return (1);
	if (grants_to_me(coder, coder->right_dongle) == 0)
		return (1);
	return (0);
}

int	predicate(t_coder *coder)
{
	if (cooldown_blocking(coder) == 1)
		return (1);
	if (other_blocking(coder) == 1)
		return (1);
	return (0);
}

static long	most_available_at(t_coder *coder)
{
	long	target;

	target = coder->data->dongles[coder->right_dongle].available_at;
	if (coder->data->dongles[coder->left_dongle].available_at > target)
		target = coder->data->dongles[coder->left_dongle].available_at;
	return (target);
}

void	wait_for_dongles(t_coder *coder)
{
	struct timespec	deadline;
	long			us;

	if (cooldown_blocking(coder) == 1 && other_blocking(coder) == 0)
	{
		us = most_available_at(coder);
		us += coder->data->start_time_us;
		deadline.tv_sec = us / 1000000;
		deadline.tv_nsec = (us % 1000000) * 1000L;
		pthread_cond_timedwait(&coder->data->condvar,
			&coder->data->dongle_mutex, &deadline);
	}
	else
		pthread_cond_wait(&coder->data->condvar, &coder->data->dongle_mutex);
}
