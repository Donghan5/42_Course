#include "push_swap.h"

/*
	The top element of the stack is sent to the bottom
	The reason we use double pointer to modify the head node
	tmp = temporary for the stability
	last = last node of the list.
*/
void	rotate(t_node **stack)
{
	t_node	*head;
	t_node	*last;

	if (ft_lstsize(*stack) < 2)
		return ;
	head = *stack;
	last = ft_lstlast(head);
	*stack = head->next;
	head->next = NULL;
	last->next = head;
}

/* rotate stack a*/
void	ra(t_node **a)
{
	rotate(a);
	ft_printf("ra\n");
}

/* rotate stack b */
void	rb(t_node **b)
{
	rotate(b);
	ft_printf("rb\n");
}

/* rotate stack a and stack b in the same time */
void	rr(t_node **a, t_node **b)
{
	rotate(a);
	rotate(b);
	ft_printf("rr\n");
}
