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

// return the longer of the reject
size_t	ft_strcspn(const char *s, const char *reject)
{
	size_t	idx;

	idx = 0;
	// when the s is not null
	while (s[idx] != '\0')
	{
		// search element of the s(string) inside of the reject
		if (ft_strchr(reject, s[idx]))
			break ; // search is done, break to exit the loop
		idx++;
	}
	return (idx);
}

/* "If there is a character in str2 that matches any character in str1,
it returns the number of characters
read until the first matching character is reached
(i.e., the position of that character - 1).
If there is no match, it returns the length of str1."
the goal of this function
Renvoie la longueur de la plus grande sous-chaîne
(en partant du début de la chaîne initiale)
ne contenant aucun des caractères spécifiés dans la liste des caractères en rejet.
*/
