/* During exam no norm rule */
#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	int idx;

	idx = 0;
	while (s[idx] != '\0')
	{
		if (s[idx] == c)
			return ((char *)s);
		idx++;
	}
	return (NULL);
}
size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t	idx;

	idx = 0;
	while (s[idx] != '\0')
	{
		if (ft_strchr(reject, s[idx]))
			break ;
		idx++;
	}
	return (idx);
}

/* "If there is a character in str2 that matches any character in str1,
it returns the number of characters
read until the first matching character is reached
(i.e., the position of that character - 1).
If there is no match, it returns the length of str1." */
