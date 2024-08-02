/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:34:52 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 11:35:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
**	initialize all element in the stack
**	all new element add in the back of stack
**	new = new node(or element)in the stack
**	args = element which in the argument value
**	i = index
*/
static void	init_stack(t_node **stack, int ac, char **av)
{
	t_node	*new;
	char	**args;
	int		i;

	i = 0;
	if (ac == 2)
		args = ft_split(av[1], ' ');
	else
	{
		i = 1;
		args = av;
	}
	while (args[i])
	{
		new = ft_lstnew(ft_atoi(args[i]));
		ft_lstadd_back(stack, new);
		i++;
	}
	index_stack(stack);
	if (ac == 2)
		free_dptr(args);
}

/* Do simple sorting by size of the stack */
static void	push_swap(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) <= 5)
		simple_sorting(a, b);
	else
		radix_sort(a, b);
}

int	main(int ac, char **av)
{
	t_node	**a;
	t_node	**b;

	if (ac < 2)
		return (ft_printf("Make sure of the arguement line\n"), -1);
	ft_check_arg(ac, av);
	a = (t_node **)malloc(sizeof(t_node));
	b = (t_node **)malloc(sizeof(t_node));
	*a = NULL;
	*b = NULL;
	init_stack(a, ac, av);
	if (is_sorted(a))
	{
		ft_free_stack(a);
		ft_free_stack(b);
		return (0);
	}
	push_swap(a, b);
	ft_free_stack(a);
	ft_free_stack(b);
	return (0);
}
