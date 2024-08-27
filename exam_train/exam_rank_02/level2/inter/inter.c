/* exam no norm rule */
#include <unistd.h>

void ft_putchar(char c)
{
	write(1, &c, 1);
}

/* to verify the char which is already appeared
if the val is 1, c_str1 is appeared in s2, set to 0 to never print again */
void	check_ascii_tab(int *ascii_tab, char c_str1)
{
	if (ascii_tab[(int)c_str1] == 1)
	{
		ascii_tab[(int)c_str1] = 0;
		ft_putchar(c_str1);
	}
}

void	inter(char *s1, char *s2)
{
	int idx;
	int ascii_tab[256];

	idx = 0;
	// initialize the value of the ascii tab to 0
	while (idx < 256)
	{
		ascii_tab[idx] = 0;
		idx++;
	}
	idx = 0;
	while (s2[idx] != '\0')
	{
		ascii_tab[(int)s2[idx]] = 1;
		idx++;
	}
	idx = 0;
	while (s1[idx] != '\0')
	{
		check_ascii_tab(ascii_tab, s1[idx]);
		idx++;
	}
}

int main(int argc, char **argv)
{
	if (argc == 3)
		inter(argv[1], argv[2]);
	ft_putchar('\n');
	return (0);
}

/* have to re-study, doesn't understand the reason using ascii_tab */
