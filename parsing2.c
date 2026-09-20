/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:35:24 by mben-mer          #+#    #+#             */
/*   Updated: 2026/09/17 16:36:17 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static const char	*arg_name(int index)
{
	static const char	*names[7] = {
		"number_of_coders",
		"time_to_burnout",
		"time_to_compile", "time_to_debug",
		"time_to_refactor", "number_of_compiiles_required",
		"dongle_cooldown"};

	if (index < 0 || index > 6)
		return ("unknown");
	return (names[index]);
}

void	print_usage_error(int ac, char **argv)
{
	fprintf(stderr, "Error: expected 8 arguments, got %d\n", ac - 1);
	fprintf(stderr, "Usage: %s number_of_coders "
		"time_to_burnout time_to_compile "
		"time_to_debug time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler\n", argv[0]);
}

void	print_invalid_number_error(const char *name, char *argv)
{
	fprintf(stderr, "Error: %s must be a positive integer "
		"(max 9 digits), got \"%s\"\n", name, argv);
}

int	validate_arguments(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (is_valid_number(argv[i]) == -1)
		{
			print_invalid_number_error(arg_name(i - 1), argv[i]);
			return (1);
		}
		i++;
	}
	if (atoi(argv[1]) == 0)
	{
		fprintf(stderr, "Error: numbers_of_coders must be greater "
			"than 0, got \"%s\"\n", argv[1]);
		return (1);
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		fprintf(stderr, "Error: scheduler must be \"fifo\" "
			"or \"edf\", got \"%s\"\n", argv[8]);
		return (1);
	}
	return (0);
}
