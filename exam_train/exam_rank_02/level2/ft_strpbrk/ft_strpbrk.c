/* exam no norm rule */
#include <unistd.h>

char *ft_strchr(const char *s, int c)
{
	size_t idx = 0;

	while (s[idx] != '\0')
	{
		if (s[idx] == c)
			return ((char *)s);
		idx++;
	}
	return (NULL);
}

char *ft_strpbrk(const char *s1, const char *s2)
{
	while (*s1)
	{
		if (ft_strchr(s2, *s1))
			return ((char *)s1);
		s1++;
	}
	return (NULL);
}

/* the func locates in the null-terminated string s(s1)
the first occurence(same) of any character in the string charset(s2)
returns a pointer to this character (man strpbrk)*/
