/* Exam no norm */
#include <stdlib.h>

// to copy the arrays ot out(double char pointer)
char *ft_strncpy(char *s1, char *s2, int n)
{
	int i = -1;
	while (++i < n && s2[i] != '\0')
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char	*ft_split(char *str)
{
	int i = 0;
	int start = 0; // index of the start of the word
	int k = 0; // index of the double array
	int wc = 0; // word count

	while (str[i])
	{
		// pass the space or tab or newline character
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		// if the str[i] increment the wc(word count)
		if (str[i]) // using the if condition not while loop
			wc++;
		// when str[i] isn't space tab or newline character, increment the index
		while (str[i] && (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'))
			i++;
	}
	// declare the double pointer of char to stock each word
	char **out = (char **)malloc(sizeof(char *) * (wc + 1));
	// initialize i = 0
	i = 0;
	while (str[i])
	{
		// pass the space tab or newline
		while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
			i++;
		// setting start
		start = i;
		// during the word (I assume)
		while (str[i] && (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'))
			i++;
		// if i is bigger than start
		if (i > start)
		{
			// declare element of the out (in sigle array)
			// beware of the length --> (i - start) + 1
			out[k] = (char *)malloc(sizeof(char) * ((i - start) + 1));
			// copy each word out[k++], &str[start], and length(i - start)
			ft_strncpy(out[k++], &str[start], i - start);
		}
	}
	// don't forget to set the last element to null
	out[k] = NULL;
	return (out);
}
