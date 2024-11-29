/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:16:21 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:16:22 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

Point::Point(void): _x(0), _y(0) {
	// std::cout << "Point Created (Default)" << std::endl;
}

Point::Point(const float x, const float y): _x(x), _y(y) {
	// std::cout << "Point Created (two float num and init)" << std::endl;
}

Point::Point(const Point &src): _x(src._x), _y(src._y) {
	// std::cout << "Point Created (Copy)" << std::endl;

}

Point::~Point(void) {
	// std::cout << "Point Destroyed" << std::endl;

}

Point	&Point::operator=(const Point &rightSide) {
	(Fixed)this->_x = rightSide.getX();
	(Fixed)this->_y = rightSide.getY();
	return (*this);
}

Fixed	Point::getX(void) const {
	return (this->_x);
}

Fixed	Point::getY(void) const {
	return (this->_y);
}
