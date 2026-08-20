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
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_request	t_request;
typedef struct s_heap	t_heap;

struct s_request
{
	int id;
	long key;
};

struct s_heap
{
	int size;
	t_request *tab;
	int capacity;
};

struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int 			fifo;
	int 			edf;
	int				stop;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	pthread_t		*threads;
	pthread_t		monitor;
	pthread_mutex_t	dongle_mutex;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	condvar;
};

struct s_coder
{
	int				id;
	int				left_dongle;
	int				right_dongle;
	int				nb_compiles;
	long			last_compile_start;
	pthread_mutex_t	coder_mutex;
	t_data			*data;
};

struct s_dongle
{
	int				id;
	int				taken_by;
	long			available_at;
	t_heap	heap;
};


int	is_digit(char c);
int	validate_arg_int(char *arg);
void	display_error_length_args(int ac, char **argv);
void	display_error_length_args(int ac, char **argv);
int	validate_arguments(int ac, char **argv);
long	current_time();
void    sleep_time(long sleep);
long	timestamp(long start_time);
int	init_data(t_data *data, char **argv);
int	init_coders(t_data *data);
int	init_dongles(t_data *data);
int malloc_threads(t_data *data);
void	create_threads(t_data *data);
void create_monitor(t_data *data);
void	join_threads(t_data *data);
int		init_structures(t_data *data, char **argv);
void	free_structures(t_data *data);
void	one_coder_in_table(t_coder *coder);
int		try_take_dongles(t_coder *coder);
void	coder_is_compiling(t_coder *coder);
void	drop_dongles(t_coder *coder);
void	coder_is_debugging(t_coder *coder);
void	coder_is_refactoring(t_coder *coder);
void	*routine(void *arg);
int		init_heap(t_heap *heap, int capacity);
int		insertion(t_heap *heap, t_request *request);
int		consultation(t_heap *heap, t_request *out);
int		extract_min(t_heap *heap, t_request *out);
void 	*monitor_routine(void *arg);

#endif
