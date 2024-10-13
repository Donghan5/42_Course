#include <unistd.h>

int negative_whitespace(const char *str, int *ptr)
{
    int sign;
    int i;

    sign = 1;
    i = 0;
    while((str[i] >= 9 && str[i] <= 13) || (str == ' '))
        i++;
    while(str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
        {
            sign *= -1;
        }
        i++;
    }
    *ptr = i;
    return (sign);
}

int ft_atoi(const char *str)
{
    int res;
    int sign;
    int i;

    res = 0;
    sign = negative_whitesapce(str, &i);
    i = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + (str[i] - 48);
    }
    return (sign * res);
}