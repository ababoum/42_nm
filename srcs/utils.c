#include "../inc/nm.h"

size_t ft_strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return (len);
}

void print_err(char *str)
{
    write(2, str, ft_strlen(str));
}