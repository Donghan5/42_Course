/* exam no norm rule */
#include <unistd.h>

// to calculate the length of the string
int	ft_strlen(char *str)
{
	int i = 0;

	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strrev(char *str)
{
	int	idx = 0;
	int len = ft_strlen(str) - 1;
	char	tmp;

	// swap the last thing and the first one idx is increment, len is decrement
	while (idx < len)
	{
		tmp = str[idx];
		str[idx] = str[len];
		str[len] = tmp;
		idx++;
		len--;
	}
	return (str);
}

/* len is must minus 1 and declare tmp to swap
and the condition of the while is less then len idx increment
len decrement make a mind, tmp is (char) NOT (char *) */
