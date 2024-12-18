/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:43:21 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 12:46:35 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	updating the count
	@param
		str: string to verify
		count: counting index (type count)
*/
static void	update_count(char *str, t_count *count)
{
	if (ft_strchr(str, '\n'))
		str = ft_strtrim(str, "\n");
	if (str[0] == 'A' && str[1] == '\0')
		count->a_count++;
	else if (str[0] == 'C' && str[1] == '\0')
		count->c_count++;
	else if ((str[0] == 'L' && str[1] == '\0') || \
		(str[0] == 'l' && str[1] == '\0'))
		count->l_count++;
	else if (str[0] == 'p' && str[1] == 'l' && str[2] == '\0')
		count->pl_count++;
	else if (str[0] == 's' && str[1] == 'p' && str[2] == '\0')
		count->sp_count++;
	else if (str[0] == 'c' && str[1] == 'y' && str[2] == '\0')
		count->cy_count++;
	else if (str[0] == 'c' && str[1] == 'o' && str[2] == '\0')
		count->co_count++;
}

/*
	check multiple declaration of the type
	@param
		sep: separated map info which want to check
		i: index
		count: structure of count which contain a, c, l etc...
*/
static void	check_double_type(char **sep)
{
	int		i;
	t_count	count;

	i = 0;
	init_count(&count);
	while (sep[i] != NULL)
	{
		update_count(sep[i], &count);
		i++;
	}
	if (count.a_count > 1 || count.c_count > 1 || count.l_count > 1 \
		|| count.pl_count > 1 || count.sp_count > 1 || count.cy_count > 1 \
		|| count.co_count > 1)
	{
		free_doub_array(sep);
		handle_error("Multiple declaration of type in single line");
	}
}

/*
	set the value type
	@param
		sep: separated information of the map_info
	@return
		type: type value which correspond each type
*/
static int	set_type(char **sep)
{
	int	type;

	if (sep[0][0] == 'A' && sep[0][1] == '\0')
		type = 1;
	else if (sep[0][0] == 'C' && sep[0][1] == '\0')
		type = 2;
	else if ((sep[0][0] == 'L' && sep[0][1] == '\0') || \
		(sep[0][0] == 'l' && sep[0][1] == '\0'))
		type = 3;
	else if (sep[0][0] == 'p' && sep[0][1] == 'l' && sep[0][2] == '\0')
		type = 4;
	else if (sep[0][0] == 's' && sep[0][1] == 'p' && sep[0][2] == '\0')
		type = 5;
	else if (sep[0][0] == 'c' && sep[0][1] == 'y' && sep[0][2] == '\0')
		type = 6;
	else if (sep[0][0] == 'c' && sep[0][1] == 'o' && sep[0][2] == '\0')
		type = 7;
	else
		return (0);
	return (type);
}

/*
	get value type
	@param
		map_info: information of the map
	@return
		type (1 to 7): type value which correspond each type
		0: fail
*/
int	get_type(char *map_info)
{
	char	**sep;
	int		type;

	type = 0;
	sep = ft_split(map_info, ' ');
	if (sep == NULL)
		return (printf("%s\n", PARSE_ERR), 0);
	if (sep[0] == NULL)
		return (free_doub_array(sep), printf("Missing type"), 0);
	check_double_type(sep);
	type = set_type(sep);
	if (type == 0)
		return (free_doub_array(sep), 0);
	free_doub_array(sep);
	return (type);
}
