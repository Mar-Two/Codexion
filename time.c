#include "header.h"

long    current_time()
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
        printf("%ld\n", current_time());
        usleep(100);
    }

}

int main()
{
    sleep_time(200);
}
