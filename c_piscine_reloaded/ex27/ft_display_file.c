/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:16:47 by donghank          #+#    #+#             */
/*   Updated: 2024/05/18 14:45:44 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

void	ft_putchar(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i], fd);
		i++;
	}
}

int	char_show(char **argv)
{
	int		fd;
	int		ret;
	char	temp[1];

	ret = 1;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr("Cannot read file.\n", 2);
		return (1);
	}
	ret = read(fd, temp, 1);
	while (ret > 0)
	{
		ft_putchar(temp[0], 1);
		ret = read(fd, temp, 1);
	}
	if (ret == -1)
	{
		close(fd);
		return (1);
	}
	if (close(fd) == -1)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 1)
		ft_putstr("File name missing.\n", 2);
	else if (argc == 2)
	{
		if (char_show(argv) == 1)
			return (1);
	}
	else
	{
		ft_putstr("Too many arguments.\n", 2);
	}
	return (0);
}
