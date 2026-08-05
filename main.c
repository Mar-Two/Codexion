#include <stdio.h>

int valide(char ***av)
{
    int i = 0;
    while(av[i])
    {
        printf("%s", (*av[i]));
        i++;
    }
    return 0;
}

int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}


int main()
{
    char fifo[5] = "fifo";
    char test[5] = "FIFO";
    if (fifo == test)
        printf("C'est haar");
}