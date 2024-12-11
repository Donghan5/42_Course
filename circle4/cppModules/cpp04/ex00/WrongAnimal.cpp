/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:21:07 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:30:52 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongAnimal.hpp"

/*
	Default constructor
*/
WrongAnimal::WrongAnimal(): type("WrongAnimal") {
	std::cout << "WrongAnimal default constructor has been called." << std::endl;
}

/*
	Type constructor
	@param
		type: type of the animal
*/
WrongAnimal::WrongAnimal(const std::string &type) {
	std::cout << "WrongAnimal name constructor has been called." << std::endl;
	this->type = type;
}

/*
	Copy constructor
	@param
		rightSide: copy obj
*/
WrongAnimal::WrongAnimal(const WrongAnimal &rightSide) {
	std::cout << "WrongAnimal copy constructor has been called." << std::endl;
	*this = rightSide;
}

/*
	Copy assignement
	@param
		rightSide: copy obj
	@return
		this: self-pointer
*/
WrongAnimal	&WrongAnimal::operator=(const WrongAnimal &rightSide) {
	this->type = rightSide.getType();
	return (*this);
}

/*
	Destructor
*/
WrongAnimal::~WrongAnimal() {
	std::cout << "Destructor of WrongAnimal has been called." << std::endl;
}

/*
	getter
	@return
		this->type: type of animal
*/
const std::string	&WrongAnimal::getType(void) const {
	return (this->type);
}

/*
	setter
	@param
		type: type of animal
*/
void	WrongAnimal::setType(const std::string &type) {
	this->type = type;
}

/*
	function required
*/
void	WrongAnimal::makeSound(void) const {
	std::cout << " *WrongAnimal sound* " << std::endl;
}


