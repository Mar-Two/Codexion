#include "header.h"

int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int validate_arg(char *arg)
{
    int len;
    int i;

    len = strlen(arg);
    if(len > 9 || arg[0] == '\0')
        return (-1);
    i = 0;
    while(i < len)
    {
        if (is_digit(arg[i]) == 0)
        {
            return (-1);
        }
        i++;
    }
    return (0);
}

int validate_scheduler(char *arg)
{
    char fifo[5];
    char edf[4];
    int i;
    int j;
    int k;

    fifo[5] = "fifo";
    edf[4] = "edf";
    i = 0;
    j = 0;
    k = 0;
    while(i < strlen(arg) && j < strlen(fifo) && k < strlen(edf))
    {
        if (arg[i] == fifo[j])
            j++;
        else if (arg[i] == edf[k])
            k++;
        i++;
    }
    if (k == i)
        return (1);
    else if (j == i)
        return (2);
    return (0);
}

static const char	*arg_name(int index)
{
	static const char	*names[7] = {
		"number_of_coders",
		"time_to_burnout",
		"time_to_compile",
		"time_to_debug",
		"time_to_refactor",
		"number_of_compiles_required",
		"dongle_cooldown"
	};

	if (index < 0 || index > 6)
		return ("unknown");
	return (names[index]);
}

int main(int ac, char **argv)
{
    int i;
    int result;
    int j;

    if(ac != 9)
    {
        fprintf(stderr, "Error: expected 8 arguments, got %d\n", ac - 1);
        fprintf(stderr, "Usage: %s number_of_coders time_to_burnout time_to_compile "
            "time_to_debug time_to_refactor number_of_compiles_required "
             "dongle_cooldown scheduler\n", argv[0]);
        return (-1);
    }
    i = 1;
    while(i <= 7)
    {
        result = validate_arg(argv[i]);
        if (result == -1)
        {
            fprintf(stderr, "Error: %s must be a positive integer (max 9 digits), got \"%s\"\n", arg_name(i - 1),argv[i]);
            return (-1);
        }
        i++;
    }
}