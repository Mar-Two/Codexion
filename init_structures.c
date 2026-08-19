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
	if (strcmp(argv[8], "fifo") == 0)
		data->fifo = 1;
	else
		data->edf = 1;
	pthread_mutex_init(&data->print_mutex, NULL);
	return (0);
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (1);
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].left_dongle = i;
		data->coders[i].right_dongle = (i + 1) % data->number_of_coders;
		data->coders[i].last_compile_start = data->start_time;
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
	i = 0;
	while (i < data->number_of_coders)
	{
		data->dongles[i].id = i;
		data->dongles[i].taken_by = -1;
		data->dongles[i].available_at = data->start_time;
		pthread_mutex_init(&data->dongles[i].dongle_mutex, NULL);
		i++;
	}
	return (0);
}

int	create_threads(t_data *data)
{
	int	i;

	data->threads = malloc(sizeof(pthread_t) * data->number_of_coders);
	if (!data->threads)
		return (1);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&data->threads[i], NULL, routine, &data->coders[i]);
		i++;
	}
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(data->threads[i], NULL);
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
	if (create_threads(data) == 1)
		return (1);
	return (0);
}

void	free_structures(t_data *data)
{
	free(data->dongles);
	free(data->coders);
	free(data);
}
