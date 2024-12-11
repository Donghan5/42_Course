/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:23 by donghank          #+#    #+#             */
/*   Updated: 2024/11/21 20:23:24 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal(): type("WrongAnimal") {
	std::cout << "WrongAnimal default constructor has been called." << std::endl;
}

WrongAnimal::WrongAnimal(const std::string &type) {
	std::cout << "WrongAnimal name constructor has been called." << std::endl;
	this->type = type;
}

WrongAnimal::WrongAnimal(const WrongAnimal &rightSide) {
	std::cout << "WrongAnimal copy constructor has been called." << std::endl;
	*this = rightSide;
}

WrongAnimal	&WrongAnimal::operator=(const WrongAnimal &rightSide) {
	this->type = rightSide.getType();
	return (*this);
}

WrongAnimal::~WrongAnimal() {
	std::cout << "Destructor of WrongAnimal has been called." << std::endl;
}

const std::string	&WrongAnimal::getType(void) const {
	return (this->type);
}

void	WrongAnimal::setType(const std::string &type) {
	this->type = type;
}

void	WrongAnimal::makeSound(void) const {
	std::cout << " *WrongAnimal sound* " << std::endl;
}


