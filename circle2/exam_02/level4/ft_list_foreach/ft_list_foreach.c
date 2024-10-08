/* exam no norm */
#include "ft_list.h"

// traverse the list and apply the functions
void	ft_list_foreach(t_list *begin_list, void (*f)(void *))
{
	while(begin_list)
	{
		(*f)(begin_list->data);
		begin_list = begin_list->next;
	}
}
