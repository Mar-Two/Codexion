/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:05:17 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 17:24:18 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	handle_burnout(t_data *data, int indexcoder)
{
	pthread_mutex_lock(&data->dongle_mutex);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d burned out\n", timestamp(data->start_time), indexcoder + 1);
	pthread_mutex_unlock(&data->print_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->condvar);
	pthread_mutex_unlock(&data->dongle_mutex);
}

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->dongle_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->condvar);
	pthread_mutex_unlock(&data->dongle_mutex);
}

static void	check_coders(t_data *data, int *indexcoder, int *count)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].coder_mutex);
		if (data->coders[i].nb_compiles >= data->number_of_compiles_required)
			(*count)++;
		if ((timestamp_us(data->start_time_us)
				- data->coders[i].last_compile_start) / 1000
			> data->time_to_burnout)
			*indexcoder = i;
		pthread_mutex_unlock(&data->coders[i].coder_mutex);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		count;
	int		indexcoder;

	data = (t_data *) arg;
	indexcoder = -1;
	while (1)
	{
		count = 0;
		check_coders(data, &indexcoder, &count);
		if (count == data->number_of_coders)
		{
			stop_simulation(data);
			break ;
		}
		else if (indexcoder != -1)
		{
			handle_burnout(data, indexcoder);
			break ;
		}
		sleep_time(1);
	}
	return (NULL);
}
