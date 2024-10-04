typedef struct  s_point
{
	int				x;
	int				y;
}					t_point;

// tab = 2D array representing gray / size = the dimension of the grid / cur = current point
void	make_flood_fill(char **tab, t_point size, t_point cur, char to_fill)
{
	// all of the error cases out of the range and the different char with want to fill
	if (cur.x < 0 || cur.y < 0 || cur.x >= size.x || cur.y >= size.y || tab[cur.y][cur.x] != to_fill)
		return ;
	tab[cur.y][cur.x] = 'F'; // initalize 'F'

	// Recursive calls for adjacent(인접한) cells
	make_flood_fill(tab, size, (t_point){cur.x - 1, cur.y}, to_fill);
	make_flood_fill(tab, size, (t_point){cur.x + 1, cur.y}, to_fill);
	make_flood_fill(tab, size, (t_point){cur.x, cur.y - 1}, to_fill);
	make_flood_fill(tab, size, (t_point){cur.x, cur.y + 1}, to_fill);
}

void	flood_fill(char **tab, t_point size, t_point begin)
{
	make_flood_fill(tab, size, begin, tab[begin.y][begin.x]);
}
