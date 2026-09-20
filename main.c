/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:03:37 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 16:04:47 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	main(int ac, char **argv)
{
	t_data	*data;

	data = NULL;
	if (ac != 9)
	{
		print_usage_error(ac, argv);
		return (1);
	}
	if (validate_arguments(argv) == 1)
		return (1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	memset(data, 0, sizeof(t_data));
	if (init_structures(data, argv) == 1)
	{
		free_structures(data);
		return (1);
	}
	create_threads(data);
	create_monitor(data);
	join_threads(data);
	free_structures(data);
	return (0);
}
