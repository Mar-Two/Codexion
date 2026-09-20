/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 01:04:42 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 16:30:49 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_valid_number(char *arg)
{
	int	len;
	int	i;

	len = strlen(arg);
	if (len > 9 || arg[0] == '\0')
		return (-1);
	i = 0;
	while (i < len)
	{
		if (is_digit(arg[i]) == 0)
		{
			return (-1);
		}
		i++;
	}
	return (0);
}
