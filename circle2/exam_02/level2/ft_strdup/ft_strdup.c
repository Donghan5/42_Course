/* Exam no norm rule */
#include <stdlib.h>
#include <stddef.h>

size_t ft_strlen(char *src)
{
	size_t	idx;

	idx = 0;
	while (src[idx] != '\0')
		idx++;
	return (idx);
}

char *ft_strdup(char *src)
{
	char *new;
	size_t	len = ft_strlen(src);
	size_t	idx;

	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	while (idx < len)
	{
		new[idx] = src[idx];
		idx++;
	}
	 // don't forget to add the null char at the end of the string copied
	new[idx] = '\0';
	return (new);
}
