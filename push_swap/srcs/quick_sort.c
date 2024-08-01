#include "push_swap.h"

/* Doing the simple swap */
static void	do_swap(int *a, int *b)
{
	int	*tmp;

	tmp = a;
	a = b;
	b = tmp;
}

/*
**	choose the pivot (median value)
**	arr = array to stock all of the stack
*/
static int	choose_pivot(t_node *stack, int size)
{
	int		*arr;
	t_node	*tmp;
	int		i;
	int		j;
	int		pivot;

	arr = (int *)malloc(sizeof(int) * size);
	i = 0;
	tmp = stack;
	while (i < size)
	{
		arr[i++] = tmp->value;
		tmp = tmp->next;
	}
	i = 0;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
			do_swap(&arr[i], &arr[j]);
	}
	pivot = arr[size / 2];
	free(arr);
	return (pivot);
}

/*
**	split the stack by pivot value
**	pivot = value of the median, criteria
**	pushed = flag to check the push
*/
static int	split_stack(t_node **a, t_node **b, int size)
{
	int	pivot;
	int	pushed;

	pivot = choose_pivot(*a, size);
	pushed = 0;
	while (pushed <= pivot)
	{
		if ((*a)->value <= pivot)
		{
			pb(a, b);
			pushed++;
		}
		else
			ra(a);
	}
	return (pushed);
}

/*
**	sorting the stack
**	b_size = size of the stack b
**	a_size = size of the stack a
*/
static void	sort_stack(t_node **a, t_node **b, int size)
{
	int	b_size;

	if (size <= 5)
	{
		simple_sorting(a, b);
		return ;
	}
	split_stack(a, b, size);
	b_size = split_stack(a, b, size);
	while (b_size--)
		pa(a, b);
	sort_stack(a, b, (size - (size - b_size)));
}

void	quick_sort(t_node **a, t_node **b)
{
	int	size;

	size = ft_lstsize(*a);
	ft_printf("Starting sort with size: %d\n", size);
	sort_stack(a, b, size);
	ft_printf("Completed\n");
}
