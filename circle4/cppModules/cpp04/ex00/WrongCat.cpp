/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:21:13 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:30:59 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongCat.hpp"

/*
	Default constructor
*/
WrongCat::WrongCat(void): WrongAnimal("WrongCat") {
	std::cout << "Default constructor of " << this->type << " has been called" << std::endl;
}

/*
	copy constructor
*/
WrongCat::WrongCat(const WrongCat &rightSide): WrongAnimal(rightSide) {
	std::cout << "Copy Constructor of " << this->type << " has been called." << std::endl;
}

WrongCat::~WrongCat() {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
}

void	WrongCat::makeSound(void) const {
	std::cout << "* Meow (WrongCat) *" << std::endl;
}
