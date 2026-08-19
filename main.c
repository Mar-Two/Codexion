#include "header.h"


int	main(int ac, char **argv)
{
	t_data	*data;
	data = NULL;

	if (validate_arguments(ac, argv) == -1)
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
	pthread_mutex_destroy(&data->print_mutex);
	free_structures(data);
	return (0);
}