/* Exam no norm */
#include <stdlib.h>

char	*ft_strncpy(char *s1, char *s2, int n)
{
	int i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char	**ft_split(char *str)
{
	int i = 0;
	int j = 0; // index of the start...?
	int k = 0; // index of the splitted output (double array)
	int word_count = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'))
			i++;
		while (str[i]) // inside of the char
			word_count++;
		while (str[i] && str[i] != ' ' || str[i] != '\n' || str[i] != '\t')
			i++;
	}
	char **out = (char **)malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'))
			i++;
		j = i; // setting the start
		while (str[i] && (str[i] != ' ' || str[i] != '\n' || str[i] != '\t'))
			i++;
		while (i > j)
		{
			out[k] = (char *)malloc(sizeof(char) * (i - j + 1));
			ft_strncpy(out[k++], &str[j], i - j);
		}
		out[k] = NULL;
	}
	return (out);
}
