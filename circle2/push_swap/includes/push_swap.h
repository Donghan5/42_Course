/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:36:14 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 23:41:55 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

/*---------- includes ----------*/
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"

/*---------- node element ----------*/
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

/*---------- push.c ----------*/
int		push(t_node **stack_from, t_node **stack_to);
int		pa(t_node **a, t_node **b);
int		pb(t_node **a, t_node **b);

/*---------- swap.c ----------*/
int		swap(t_node **stack);
int		sa(t_node **a);
int		sb(t_node **b);
int		ss(t_node **a, t_node **b);

/*---------- rotate.c ----------*/
int		rotate(t_node **stack);
int		ra(t_node **a);
int		rb(t_node **b);
int		rr(t_node **a, t_node **b);

/*----------- reverse_rotate.c ----------*/
int		reverse_rotate(t_node **stack);
int		rra(t_node **a);
int		rrb(t_node **b);
int		rrr(t_node **a, t_node **b);

/*---------- sort.c ----------*/
void	sort_five_element(t_node **a, t_node **b);
void	simple_sorting(t_node **a, t_node **b);

/*---------- index.c ----------*/
void	index_stack(t_node **stack);

/*---------- radix_sort.c ----------*/
void	radix_sort(t_node **a, t_node **b);

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
