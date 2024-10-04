/* During the exam session no norm rules */

int ft_strcmp(char *s1, char *s2)
{
	int	idx = 0;

	// when s1 and s2 is same and also not a value of the NULL, advance index
	while ((s1[idx] == s2[idx]) && (s1[idx] != '\0' && s2[idx] != '\0'))
		idx++;
	// have to cast to unsigned char type
	return ((unsigned char)s1[idx] - (unsigned char)s2[idx]);
}
