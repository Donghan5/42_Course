/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:54:06 by donghank          #+#    #+#             */
/*   Updated: 2024/12/17 14:27:48 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/*
	check ambient block information
	A    double    int, int, int
	@param
		map_info: information of the map
		i: pointer index
*/
void	validity_type_one(char *map_info, size_t *i, int fd)
{
	check_doub(fd, map_info, i);
	check_three(fd, map_info, i, "int");
}

/*
	check camera information block
	C    int, int, int    double, double, double    int
	@param
		map_info: information of the map
		i: pointer index
*/
void	validity_type_two(char *map_info, size_t *i, int fd)
{
	check_three(fd, map_info, i, "int");
	check_three(fd, map_info, i, "double");
	check_int(fd, map_info, i);
}

/*
	check three block of the information
	[L, sp]   double, double, double    double    int, int, int
	@param
		map_info: information of the map
		i: pointer index
*/
void	validity_type_three(char *map_info, size_t *i, int fd)
{
	check_three(fd, map_info, i, "double");
	check_doub(fd, map_info, i);
	check_three(fd, map_info, i, "int");
}

/*
	to check four block of the information
	pl    double, double, double    double, double, double    int, int, int
	@param
		map_info: information of the map
		i: pointer index
*/
void	validity_type_four(char *map_info, size_t *i, int fd)
{
	check_three(fd, map_info, i, "double");
	check_three(fd, map_info, i, "double");
	check_three(fd, map_info, i, "int");
}

/*
	to check five block of the information
	[cy, co]    double, double, double    double, double, double
			   double    double    int, int, int
	@param
		map_info: information of the map
		i: pointer index
*/
void	validity_type_five(char *map_info, size_t *i, int fd)
{
	check_three(fd, map_info, i, "double");
	check_three(fd, map_info, i, "double");
	check_doub(fd, map_info, i);
	check_doub(fd, map_info, i);
	check_three(fd, map_info, i, "int");
}
