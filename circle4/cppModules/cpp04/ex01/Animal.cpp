/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:22:05 by donghank          #+#    #+#             */
/*   Updated: 2024/12/06 15:11:55 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

/*
	Default constructor
*/
Animal::Animal(void): type("Animal") {
	std::cout << "Animal default constructor has been called." << std::endl;
}

/*
	Type constructor
	@param
		type: type of animal
*/
Animal::Animal(const std::string &type) {
	std::cout << "Type constructor of Animal has been called." << std::endl;
	this->type = type;
}

/*
	Copy constructor
	@param
		rightSide: copy objs
*/
Animal::Animal(const Animal &rightSide) {
	std::cout << "Copy constructor of Animal has been called." << std::endl;
	*this = rightSide;
}

/*
	Copy assignement
	@param
		rightSide: copy objs
	@return
		this: self-pointer of the class
*/
Animal	&Animal::operator=(const Animal &rightSide) {
	this->type = rightSide.getType();
	return (*this);
}

/*
	Destructor
*/
Animal::~Animal() {
	std::cout << "Destructor of Animal has been called." << std::endl;
}

/*
	getter
	@return
		this->type: type of the class(obj)
*/
const std::string	&Animal::getType(void) const {
	return(this->type);
}

/*
	setter
	@param
		type: type of animal
*/
void	Animal::setType(const std::string &type) {
	this->type = type;
}

/*
	required function
*/
void	Animal::makeSound(void) const {
	std::cout << " * Animal sound * " << std::endl;
}
