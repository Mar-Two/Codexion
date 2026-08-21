#include "header.h"

long	current_time(void)
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

long	current_time_us(void)
{
	struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000l) + (tv.tv_usec);
}

long	timestamp_us(long start_time_us)
{
	long result;
	result = current_time_us() - start_time_us;
	return (result);
}