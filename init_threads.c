#include "header.h"

int alloc_threads(t_data *data)
{
	data->threads = malloc(sizeof(pthread_t) * data->number_of_coders);
	if (!data->threads)
		return (1);
	return (0);
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
