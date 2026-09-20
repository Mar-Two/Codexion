/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eligibility.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:00:00 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 18:00:00 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_servable(t_coder *c)
{
	long	now;

	now = timestamp_us(c->data->start_time_us);
	if (c->data->dongles[c->left_dongle].taken_by != -1)
		return (0);
	if (c->data->dongles[c->right_dongle].taken_by != -1)
		return (0);
	if (now < c->data->dongles[c->left_dongle].available_at)
		return (0);
	if (now < c->data->dongles[c->right_dongle].available_at)
		return (0);
	return (1);
}

int	grants_to_me(t_coder *coder, int d)
{
	t_request	head;

	if (heap_peek(&coder->data->dongles[d].heap, &head) == 1)
		return (1);
	if (head.id == coder->id)
		return (1);
	return (!is_servable(&coder->data->coders[head.id - 1]));
}
