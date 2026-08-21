#include "header.h"

int	init_data(t_data *data, char **argv)
{
	data->fifo = 0;
	data->edf = 0;
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	data->start_time = current_time();
	data->start_time_us = current_time_us();
	data->stop = 0;
	if (strcmp(argv[8], "fifo") == 0)
		data->fifo = 1;
	else
		data->edf = 1;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->dongle_mutex, NULL);
	pthread_cond_init(&data->condvar, NULL);
	return (0);
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (1);
	memset(data->coders, 0, sizeof(t_coder) * data->number_of_coders);
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = i;
		data->coders[i].right_dongle = (i + 1) % data->number_of_coders;
		data->coders[i].last_compile_start = 0;
		data->coders[i].nb_compiles = 0;
		data->coders[i].data = data;
		pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
		i++;
	}
	return (0);
}

int	init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (1);
	memset(data->dongles, 0, sizeof(t_dongle) * data->number_of_coders);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].taken_by = -1;
		data->dongles[i].available_at = 0;
		if(init_heap(&data->dongles[i].heap, data->number_of_coders) == 1)
			return 1;
		i++;
	}
	return (0);
}

int	init_structures(t_data *data, char **argv)
{
	if (init_data(data, argv) == 1)
		return (1);
	if (init_coders(data) == 1)
		return (1);
	if (init_dongles(data) == 1)
		return (1);
	if (malloc_threads(data) == 1)
		return (1);
	return (0);
}

void	free_structures(t_data *data)
{
	int i;

	i = 0;
	while(i < data->number_of_coders)
	{
		free(data->dongles[i].heap.tab);
		pthread_mutex_destroy(&data->coders[i].coder_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->dongle_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_cond_destroy(&data->condvar);
	free(data->threads);
	free(data->dongles);
	free(data->coders);
	free(data);
}

/*
static void condition_stop(t_data *data, int *indexcoder, int *count, int i)
{
	pthread_mutex_lock(&data->coders[i].coder_mutex);
	if (data->coders[i].nb_compiles >= data->number_of_compiles_required)
		(*count)++;
	if (timestamp(data->start_time) - data->coders[i].last_compile_start > data->time_to_burnout)
		*indexcoder = i;
	pthread_mutex_unlock(&data->coders[i].coder_mutex);
}

static void handle_burnout(t_data *data, int indexcoder)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d burned out\n", timestamp(data->start_time), indexcoder + 1);
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_lock(&data->dongle_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->condvar);
	pthread_mutex_unlock(&data->dongle_mutex);
}

static void stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->dongle_mutex);
	data->stop = 1;
	pthread_cond_broadcast(&data->condvar);
	pthread_mutex_unlock(&data->dongle_mutex);
}

static void check_coders(t_data *data, int *indexcoder, int *count)
{
	int	i;

	i = 0;
	while(i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].coder_mutex);
		if (data->coders[i].nb_compiles >= data->number_of_compiles_required)
			(*count)++;
		if (timestamp(data->start_time) - data->coders[i].last_compile_start > data->time_to_burnout)
			*indexcoder = i;
		pthread_mutex_unlock(&data->coders[i].coder_mutex);
		i++;
	}
}

void *monitor_routine(void *arg)
{
	t_data *data;
	int count;
	int indexcoder;

	data = (t_data*) arg;
	indexcoder = -1;
	while(1)
	{
		count = 0;
		check_coders(data, &indexcoder, &count);
		if (indexcoder != -1)
		{
			handle_burnout(data, indexcoder);
			break;
		}
		else if(count == data->number_of_coders)
		{
			stop_simulation(data);
			break;
		}
		sleep_time(1);
	}
	return (NULL);
}
*/

/*
int malloc_threads(t_data *data)
{
	data->threads = malloc(sizeof(pthread_t) * data->number_of_coders);
	if (!data->threads)
		return (1);
	return 0;
}

void	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&data->threads[i], NULL, routine, &data->coders[i]);
		i++;
	}
}

void create_monitor(t_data *data)
{
	pthread_create(&data->monitor, NULL, monitor_routine, data);
}
	
void join_threads(t_data *data)
{
	int i;

	i = 0;
	pthread_join(data->monitor, NULL);
	while (i < data->number_of_coders)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}
*/

