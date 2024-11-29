/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:16:18 by donghank          #+#    #+#             */
/*   Updated: 2024/11/29 13:50:22 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"
#include "Point.hpp"

/*
	test cases
	a(0, 0), b(10, 30), c(30, 30)
	p (20, 10) ==> outside
	p (10, 10) ==> inside
	Point(30, 40), Point(20, 20), Point(30, 10), Point(27, 33.9)
	Point(0, 0), Point(10, 30), Point(30, 30), Point(10, 10)
*/
int	main(void) {
	if (bsp(Point(30, 40), Point(20, 20), Point(30, 10), Point(27, 33.9))) {
		std::cout << "Point is inside of triangle" << std::endl;
	}
	else {
		std::cout << "Point is outside of triangle" << std::endl;
	}
	return (0);
}
