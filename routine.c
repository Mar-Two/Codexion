/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:18:23 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 16:19:19 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	should_stop(t_coder	*coder)
{
	int	result;

	result = 0;
	pthread_mutex_lock(&coder->data->dongle_mutex);
	if (coder->data->stop == 1)
		result = 1;
	pthread_mutex_unlock(&coder->data->dongle_mutex);
	return (result);
}

static void	run_lonely_coder(t_coder *coder)
{
	take_single_dongle(coder);
	pthread_mutex_lock(&coder->data->dongle_mutex);
	while (coder->data->stop == 0)
	{
		pthread_cond_wait(&coder->data->condvar, &coder->data->dongle_mutex);
	}
	pthread_mutex_unlock(&coder->data->dongle_mutex);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	if (coder->left_dongle == coder->right_dongle)
	{
		run_lonely_coder(coder);
		return (NULL);
	}
	while (should_stop(coder) == 0)
	{
		if (try_take_dongles(coder) == 1)
			break ;
		if (coder_is_compiling(coder) == 1)
			break ;
		drop_dongles(coder);
		if (coder_is_debugging(coder) == 1)
			break ;
		if (coder_is_refactoring(coder) == 1)
			break ;
	}
	return (NULL);
}
