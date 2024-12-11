/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Animal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:20:30 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:23:23 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Animal.hpp"

/*
	Default constructor
*/
Animal::Animal(): type("Animal") {
	std::cout << "Animal default constructor has been called." << std::endl;
}

/*
	Type constructor
	@param
		type: type of the animal
*/
Animal::Animal(const std::string &type) {
	std::cout << "Type constructor of Animal has been called." << std::endl;
	this->type = type;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
Animal::Animal(const Animal &rightSide) {
	std::cout << "Copy constructor of Animal has been called." << std::endl;
	*this = rightSide;
}

/*
	Copy assignement
	@param
		rightSide: copy obj
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
		this->type: type od the Animal
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
	function required
*/
void	Animal::makeSound(void) const {
	std::cout << " * Animal sound * " << std::endl;
}
