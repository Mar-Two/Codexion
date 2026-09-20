/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:20:07 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 16:21:38 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	sleep_time(long sleep)
{
	long	cible;
	long	rest;

	cible = current_time_us() + (sleep * 1000);
	rest = cible - current_time_us();
	while (rest > 0)
	{
		if (rest > 1000)
			usleep(rest / 2);
		else
			usleep(50);
		rest = cible - current_time_us();
	}
}

long	timestamp(long start_time)
{
	long	result;

	result = current_time() - start_time;
	return (result);
}

long	current_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000l) + (tv.tv_usec));
}

long	timestamp_us(long start_time_us)
{
	long	result;

	result = current_time_us() - start_time_us;
	return (result);
}
