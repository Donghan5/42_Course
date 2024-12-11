/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongCat.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 20:23:29 by donghank          #+#    #+#             */
/*   Updated: 2024/12/10 12:32:03 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WrongCat.hpp"

WrongCat::WrongCat(void): WrongAnimal("WrongCat") {
	std::cout << "Default constructor of " << this->type << " has been called" << std::endl;
}

WrongCat::WrongCat(const WrongCat &rightSide): WrongAnimal(rightSide) {
	std::cout << "Copy Constructor of " << this->type << " has been called." << std::endl;
}

WrongCat::~WrongCat() {
	std::cout << "Destructor of " << this->type << " has been called." << std::endl;
}

void	WrongCat::makeSound(void) const {
	std::cout << "* Meow~~~ *" << std::endl;
}
