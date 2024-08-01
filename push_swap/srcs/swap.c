#include "push_swap.h"

/* implement the function swap */
/*
	explain parameter
	tmp_value = to swap value (stability)
	tmp_index = to swap index (stability)
	head = to point the first node
	next = to point the second node
*/
void	swap(t_node **stack)
{
	int		tmp_value;
	int		tmp_index;
	t_node	*head;
	t_node	*next;

	head = *stack;
	next = head->next;
	if (!head && !next)
		ft_printf("Swaping error\n");
	tmp_value = head->value;
	tmp_index = head->index;
	head->value = next->value;
	head->index = next->index;
	next->value = tmp_value;
	next->index = tmp_index;
}

/* swap the two first element in stack a */
void	sa(t_node **a)
{
	if (ft_lstsize(*a) < 2)
		return ;
	swap(a);
	ft_printf("sa\n");
}

/* swap the two first element in stack b */
void	sb(t_node **b)
{
	if (ft_lstsize(*b) < 2)
		return ;
	swap(b);
	ft_printf("sb\n");
}

/* swap the two first element in stack b */
void	ss(t_node **a, t_node **b)
{
	if (ft_lstsize(*a) < 2 || ft_lstsize(*b) < 2)
		return ;
	swap(a);
	swap(b);
	ft_printf("ss\n");
}
