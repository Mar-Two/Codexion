/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 01:04:42 by mben-mer          #+#    #+#             */
/*   Updated: 2026/08/06 01:38:47 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	validate_arg_int(char *arg)
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

static const char	*arg_name(int index)
{
	static const char	*names[7] = {
		"number_of_coders",
		"time_to_burnout",
		"time_to_compile", "time_to_debug",
		"time_to_refactor", "number_of_compiles_required",
		"dongle_cooldown"};

	if (index < 0 || index > 6)
		return ("unknown");
	return (names[index]);
}

void	display_error_length_args(int ac, char **argv)
{
	fprintf(stderr, "Error: expected 8 arguments, got %d\n", ac - 1);
	fprintf(stderr, "Usage: %s number_of_coders "
		"time_to_burnout time_to_compile "
		"time_to_debug time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler\n", argv[0]);
}

int	validate_arguments(int ac, char **argv)
{
	int	i;

	if (ac != 9)
	{
		display_error_length_args(ac, argv);
		return (-1);
	}
	i = 1;
	while (i <= 7)
	{
		if (validate_arg_int(argv[i]) == -1)
		{
			fprintf(stderr, "Error: %s must be a positive integer "
				"(max 9 digits), got \"%s\"\n", arg_name(i - 1), argv[i]);
			return (-1);
		}
		i++;
	}
	if (atoi(argv[1]) == 0)
	{
		fprintf(stderr, "Error: numbers_of_coders must be greater "
			"than 0, got \"%s\"\n", argv[1]);
		return (-1);
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		fprintf(stderr, "Error: scheduler must be \"fifo\" "
			"or \"edf\", got \"%s\"\n", argv[8]);
		return (-1);
	}
	return (0);
}