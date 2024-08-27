/* Exam no norm */

void	sort_int_tab(int *tab, unsigned int size)
{
	unsigned int i = 0;
	int	tmp;

	// to avoid overflow to set size - 1
	while (i < (size - 1))
	{
		if (tab[i] > tab[i + 1])
		{
			tmp = tab[i];
			tab[i] = tab[i + 1];
			tab[i + 1] = tmp;
			i = 0; // don't forget to initialize end of the loop
		}
		else
			i++;
	}
}
