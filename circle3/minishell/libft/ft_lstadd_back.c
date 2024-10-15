/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 21:06:50 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/18 21:16:03 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*i;

	i = *lst;
	while (i)
	{
		if (!i->next)
		{
			i->next = new;
			return ;
		}
		i = i->next;
	}
	if (!i)
		*lst = new;
}
