/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 18:00:00 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 18:00:00 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	passes_before(t_request *a, t_request *b)
{
	if (a->key < b->key)
		return (1);
	if (a->key > b->key)
		return (0);
	return (a->id < b->id);
}

int	sift_up(t_heap *heap, int i)
{
	t_request	tmp;
	int			parent;

	parent = (i - 1) / 2;
	while (i > 0 && passes_before(&heap->tab[i], &heap->tab[parent]))
	{
		tmp = heap->tab[i];
		heap->tab[i] = heap->tab[parent];
		heap->tab[parent] = tmp;
		i = parent;
		parent = (i - 1) / 2;
	}
	return (i);
}

static int	best_child(t_heap *heap, int i)
{
	int	best;

	best = i;
	if ((2 * i) + 1 < heap->size
		&& passes_before(&heap->tab[(2 * i) + 1], &heap->tab[best]))
		best = (2 * i) + 1;
	if ((2 * i) + 2 < heap->size
		&& passes_before(&heap->tab[(2 * i) + 2], &heap->tab[best]))
		best = (2 * i) + 2;
	return (best);
}

void	sift_down(t_heap *heap, int i)
{
	t_request	tmp;
	int			best;

	best = best_child(heap, i);
	while (best != i)
	{
		tmp = heap->tab[i];
		heap->tab[i] = heap->tab[best];
		heap->tab[best] = tmp;
		i = best;
		best = best_child(heap, i);
	}
}

int	heap_remove(t_heap *heap, int id)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->tab[i].id != id)
		i++;
	if (i == heap->size)
		return (1);
	heap->tab[i] = heap->tab[heap->size - 1];
	heap->size -= 1;
	if (i == heap->size)
		return (0);
	if (sift_up(heap, i) == i)
		sift_down(heap, i);
	return (0);
}
