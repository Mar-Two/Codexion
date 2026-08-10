/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mben-mer <mben-mer@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 17:20:22 by mben-mer          #+#    #+#             */
/*   Updated: 2026/08/06 17:20:27 by mben-mer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct s_data
{
	int		number_of_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int		number_of_compiles_required;
	long	dongle_cooldown;
	int		scheduler;
	t_coder *coders;
	t_dongle *dongles;
	long start_time;
}	t_data;

typedef struct s_coder
{
	int 	id;
	int 	left_dongle;
	int 	right_dongle;
	int 	nb_compiles;
	long last_compile_start;
	t_data *data;
} t_coder;

typedef struct s_dongle
{
	int		id;
	int		taken_by;
	long	available_at;
} t_dongle;

#endif
