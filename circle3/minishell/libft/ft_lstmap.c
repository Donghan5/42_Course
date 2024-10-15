/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pzinurov <pzinurov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 21:55:24 by pzinurov          #+#    #+#             */
/*   Updated: 2024/05/20 17:02:39 by pzinurov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*n_l;

	n_l = NULL;
	if (lst && f && del)
	{
		n_l = ft_lstnew(f(lst->content));
		new_lst = n_l;
		if (!new_lst)
			return (NULL);
		if (!new_lst->content)
			return (ft_lstclear(&n_l, del), NULL);
	}
	while (lst && f && del)
	{
		lst = lst->next;
		if (lst)
		{
			new_lst->next = ft_lstnew(f(lst->content));
			new_lst = new_lst->next;
			if (!new_lst || !new_lst->content)
				return (ft_lstclear(&n_l, del), NULL);
		}
	}
	return (n_l);
}
