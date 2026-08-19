#include "header.h"

long	current_time()
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void    sleep_time(long sleep)
{
    long cible;
    cible = current_time() + sleep;
    while(current_time() < cible)
    {
        usleep(100);
    }
}

long	timestamp(long start_time)
{
	long result;

	result = current_time() - start_time;
	return (result);
}