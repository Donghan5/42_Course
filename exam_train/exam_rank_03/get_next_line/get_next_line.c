#include "get_next_line.h"

char	*ft_strdup(char *src)
{
	char	*dst;
	int	i;

	i = 0;
	while (src[i])
		i++;
	dst = (char *)malloc(sizeof(char) * (i + 1));
	i = -1;
	while (src[++i])
		dst[i] = src[i];
	dst[i] = '\0';
	return(dst);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	char		line[70000];
	static int	buffer_readed;
	static int	buffer_pos;
	int			i;

	i = 0;
	while (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (buffer_pos >= buffer_readed)
		{
			buffer_readed = read(fd, buffer, BUFFER_SIZE);
			buffer_pos = 0;
			if (buffer_readed <= 0)
				break ;
		}
		line[i++] = buffer[buffer_pos];
		if (line[i - 1] == '\n')
			break ;
	}
	line[i] = '\0';
	if (i == 0)
		return (NULL);
	return (ft_strdup(line));
}
