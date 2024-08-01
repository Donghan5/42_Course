#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

/*---------- includes ----------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include "ft_printf.h"
# include "libft.h"

/*---------- node element ----------*/
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

/*---------- push.c ----------*/
void	push(t_node **stack_from, t_node **stack_to);
void	pa(t_node **a, t_node **b);
void	pb(t_node **a, t_node **b);

/*---------- swap.c ----------*/
void	swap(t_node **stack);
void	sa(t_node **a);
void	sb(t_node **b);
void	ss(t_node **a, t_node **b);

/*---------- rotate.c ----------*/
void	rotate(t_node **stack);
void	ra(t_node **a);
void	rb(t_node **b);
void	rr(t_node **a, t_node **b);

/*----------- reverse_rotate.c ----------*/
void	reverse_rotate(t_node **stack);
void	rra(t_node **a);
void	rrb(t_node **b);
void	rrr(t_node **a, t_node **b);

/*---------- sort.c ----------*/
void	sort_five_element(t_node **a, t_node **b);
void	simple_sorting(t_node **a, t_node **b);

/*---------- index.c ----------*/
void	index_stack(t_node **stack);

/*---------- quick_sort.c ----------*/
void	quick_sort(t_node **a, t_node **b);

/*---------- cmd_utils.c ----------*/
void	ft_check_arg(int ac, char **av);

/*---------- ft_lstutils.c ----------*/
t_node	*ft_lstlast(t_node *lst);
int		ft_lstsize(t_node *lst);
t_node	*ft_lstnew(int value);
void	ft_lstadd_back(t_node **lst, t_node *new);

/*---------- sort_utils.c ----------*/
int		is_sorted(t_node **stack);
int		get_distance_index(t_node **stack, int index);
void	make_top_element(t_node **stack, int distance);

/*---------- utils.c ----------*/
void	ft_free_stack(t_node **stack);
void	free_dptr(char **str);
void	double_free(t_node *stack1, t_node *stack2);
int		ft_atoll(const char *str);
void	error_with_exit(char *msg);

#endif
