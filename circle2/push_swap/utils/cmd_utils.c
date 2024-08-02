/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:35:36 by donghank          #+#    #+#             */
/*   Updated: 2024/08/02 13:27:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
**	to verify the command line thing is numeric
**	i = index to num(type of char)
**	if numeric return 0 if not return 1;
*/
static int	is_num_help(char *num)
{
	int	i;

	i = 0;
	if (num[0] == '\0')
		return (1);
	if (num[0] == '-' || num[0] == '+')
		i++;
	if (num[i] == '\0')
		return (1);
	while (num[i] != '\0')
	{
		if (!ft_isdigit(num[i]))
			return (1);
		i++;
	}
	return (0);
}

/*
**	to check the arguemet is the type of integers
**	num = the number of the argement line
**	i = first index(index of the argv)
**	the case of the av is num return 1 if not return 0
*/
static int	ft_element(int num, char **av, int i)
{
	i++;
	while (av[i])
	{
		if (ft_atoll(av[i]) == num)
			return (1);
		i++;
	}
	return (0);
}

/* to check the existance of whitespace inside of the argument line */
static int	ft_check_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

/*
**	to verify the validity the command line argument
**	i = index of the av
**	tmp = temporary integer to compare
**	args = to split all of the element in the argument
*/
void	ft_check_arg(int ac, char **av)
{
	int				i;
	long long		tmp;
	char			**args;

	i = 0;
	if (ac == 2)
		args = ft_split(av[1], ' ');
	else
	{
		i = 1;
		args = av;
	}
	while (args[i] != NULL)
	{
		tmp = ft_atoll(args[i]);
		if (is_num_help(args[i]) == 1)
			error_with_exit("Isn't numeric");
		if (ft_element(tmp, args, i) == 1)
			error_with_exit("Isn't type int");
		if (ft_check_whitespace(args[i]) == 1)
			error_with_exit("Not whitespace");
		i++;
	}
	if (ac == 2)
		free_dptr(args);
}
