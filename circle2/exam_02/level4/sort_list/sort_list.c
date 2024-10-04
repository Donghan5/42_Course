/* Exam no norm */
#include "list.h"
#include <stdlib.h>

// swap is to temporary to swap the datas, start is head pointer
t_list	*sort_list(t_list *lst, int (*cmp)(int, int))
{
	int	swap;
	t_list	*start;

	start = lst;
	// please to check lst and lst->next isn't NULL
	while (lst != NULL && lst->next != NULL)
	{
		// when the cmp value is 0, have to swap the elements
		if ((*cmp)(lst->data, lst->next->data) == 0)
		{
			swap = lst->data;
			lst->data = lst->next->data;
			lst->next->data = swap;
			lst = start;
		}
		//if not, advance the lst
		else
			lst = lst->next;
	}
	return (start);
}
