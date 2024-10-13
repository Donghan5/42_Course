#include <stdlib.h>

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        i++;
    }
    return (i);
}

char    *ft_strdup(char *src)
{
    int i;
    char *new_src;
    int len;

    i = 0;
    len = ft_strlen(src);
    new_src = (char *)malloc(sizeof(char) * (len + 1));
    if (!new_src)
    {
        return (0);
    }
    while (src[i] != '\0')
    {
        new_src[i] = src[i];
        i++;
    }
    new_src[i] = '\0';
    return (new_src);
}