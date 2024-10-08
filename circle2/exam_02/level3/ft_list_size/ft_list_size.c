#include "ft_list.h"

/* by using recursive function (add 1)
if begin_list = 0 is return 0 (because it is a edge case)*/
int	ft_list_size(t_list *begin_list)
{
	if (begin_list == 0)
		return (0);
	else
		return (1 + ft_list_size(begin_list->next));
}