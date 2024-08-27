/* exam no norm rule */
#include <unistd.h>

char *ft_strchr(const char *s, int c)
{
	size_t idx = 0;

	// when element of the s(string) is not null
	while (s[idx] != '\0')
	{
		// if the element of the s is equal to c
		if (s[idx] == c)
		// return the s (have to convert to char *)
			return ((char *)s);
		idx++;
	}
	// if not return NULL
	return (NULL);
}
// return the pointer of the s1 have to convert to char * type
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
