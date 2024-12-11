/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:55 by donghank          #+#    #+#             */
/*   Updated: 2024/12/11 14:13:37 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

Animal::Animal(void): type("Animal") {
	std::cout << "Animal default constructor has been called." << std::endl;
}

Animal::Animal(const std::string &type) {
	std::cout << "Type constructor of Animal has been called." << std::endl;
	this->type = type;
}

Animal::Animal(const Animal &rightSide) {
	std::cout << "Copy constructor of Animal has been called." << std::endl;
	*this = rightSide;
}

Animal	&Animal::operator=(const Animal &rightSide) {
	this->type = rightSide.getType();
	return (*this);
}

Animal::~Animal() {
	std::cout << "Destructor of Animal has been called." << std::endl;
}

const std::string	&Animal::getType(void) const {
	return(this->type);
}

void	Animal::setType(const std::string &type) {
	this->type = type;
}
