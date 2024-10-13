int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

char	*ft_strrev(char *str)
{
	int	i;
	int	len;
	char	temp;

	i = 0;
	len = ft_strlen(str);
	while (i <= (len / 2))
	{
		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
		i++;
	}
	return (str);
}

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int i;

	i = 0;
	if (argc != 2)
	{
		write(1, "\n", 1);
		return (0);
	}
	else
		printf("%s\n", ft_strrev(argv[1]));
	return (0);
}
